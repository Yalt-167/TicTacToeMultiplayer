#include "ServerSocket.hpp"
#include "GameServer.hpp"
#include "PacketSendTarget.hpp"


ServerSocket::ServerSocket() : Socket()
{
    Init();
}
ServerSocket::~ServerSocket()
{
    Cleanup();
}

void ServerSocket::Run()
{
    sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    while (true)
    {
        while (connectedClients == 2);

        SOCKET clientSocket = accept(socket_, (sockaddr*)&clientAddr, &addrLen);
        
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to accept client" << std::endl;
            continue;
        }

        if (recv(clientSocket, headerBuffer, sizeof(PacketHeader), 0) <= 0)
        {
            std::cerr << "Gone too soon" << std::endl;
            continue;
        }
        
        int nameSize = reinterpret_cast<int*>(headerBuffer)[1] + 1; // include null terminator
        char* nameBuffer = new char[nameSize];

        _ = recv(clientSocket, nameBuffer, nameSize, 0);
        
        int clientIndex = freeClientIDs[0];
        freeClientIDs.erase(freeClientIDs.begin());

        std::cout << nameBuffer << " connected" << std::endl;
        connectedClients++;

        clientSockets[clientIndex] = clientSocket;
        clientThreads[clientIndex] = std::thread(
            [=]()
            {
                HandleClient(clientSocket, nameBuffer, clientIndex);
            }
        );
        clientThreads[clientIndex].detach();
    }
}

int ServerSocket::GetConnectedClientsCount() const
{
    return connectedClients;
}

void ServerSocket::Send(const char* data, const SerializationHeaders what, const int size, const PacketSendTarget target)
{
    if (target == PacketSendTarget::Broadcast)
    {
        for (auto& [__, socket] : clientSockets)
        {
            _ = send(socket, header.Set(what, size), sizeof(PacketHeader), 0);

            _ = send(socket, data, size, 0);
        }
    }
    else
    {
        _ = send(clientSockets[static_cast<int>(target)], header.Set(what, size), sizeof(PacketHeader), 0);

        _ = send(clientSockets[static_cast<int>(target)], data, size, 0);
    }
}

void ServerSocket::Init()
{
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) // setting up as server
    {
        std::cerr << "Bind failed" << std::endl;
        Cleanup();

        throw std::exception("Bind failed");
    }

    if (listen(socket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        Cleanup();

        throw std::exception("Listen failed");
    }

    std::cout << "Server should be listening on port " << PORT << "..." << std::endl;
}

void ServerSocket::HandleClient(SOCKET clientSocket, const char* name, const int clientNumber)
{
    GameServer::RestoreChatMessages(clientNumber);

    std::string loginLog = "[Server]: " + std::string(name) + " connected";
    GameServer::StoreChatMessage(loginLog);
    Send(
        loginLog.c_str(),
        SerializationHeaders::ChatMessage,
        static_cast<int>(loginLog.size()) + 1,
        PacketSendTarget::Broadcast
    );

    char headerBuffer[sizeof(PacketHeader)];
    bool socketOpen = true;
    while (socketOpen)
    {
        if (recv(clientSocket, headerBuffer, sizeof(PacketHeader), 0) <= 0)
        {
            std::cout << "[" << name << "] disconnected" << std::endl;
            break;
        }

        PacketHeader header = *reinterpret_cast<PacketHeader*>(headerBuffer);

        if (GameServer::LogEveryPacket)
        {
            std::cout
                << "Got " << PacketHeader::LegibleSerializationHeaders(header.SerializationHeader)
                << " packet from [" << name
                << "] || Size: " << header.Size
                << std::endl;
        }
        switch (header.SerializationHeader)
        {
        case SerializationHeaders::Disconnection:
            HandleDisconnection(clientSocket, name, clientNumber);
            socketOpen = false;
            break;

        case SerializationHeaders::ChatMessage:
            HandleChatMessage(clientSocket, clientNumber, header.Size);
            break;

        case SerializationHeaders::Play:
            HandlePlay(clientSocket, clientNumber);
            break;
        }
    }
    
    delete[] name;
    
    clientThreads.erase(clientNumber);
    clientSockets.erase(clientNumber);

    _ = closesocket(clientSocket);

    freeClientIDs.push_back(clientNumber);
    connectedClients--;
}


void ServerSocket::HandleChatMessage(SOCKET& clientSocket, const int clientNumber, const int messageSize)
{
    char* chatBuffer = new char[messageSize];

    _ = recv(clientSocket, chatBuffer, messageSize, 0);

    std::cout << chatBuffer << std::endl;

    GameServer::StoreChatMessage(chatBuffer);

    if (connectedClients < 2)
    {
        std::cout << "Message not sent because there is only one client connected" << std::endl;
        delete[] chatBuffer;
        return; // why even bother you are lonely AF just give up
    }

    //int otherClientIndex = (int)(!(bool)clientNumber); // lowkey sinning 
    int otherClientIndex = clientNumber == 0 ? 1 : 0; // i was forced into the "righteous" path
    _ = send(clientSockets[otherClientIndex], header.Set(SerializationHeaders::ChatMessage, messageSize), sizeof(PacketHeader), 0);

    _ = send(clientSockets[otherClientIndex], chatBuffer, messageSize, 0);

    delete[] chatBuffer;
}

void ServerSocket::HandlePlay(SOCKET& clientSocket, const int clientNumber)
{
    char playBuffer[sizeof(int)];

    _ = recv(clientSocket, playBuffer, sizeof(int), 0);

    int returnBuffer[3]; // { <the play itself>, <wether u can play>, <who played> }

    GameServer::ParsePlay(*reinterpret_cast<int*>(playBuffer), returnBuffer, clientNumber);

    // send back to the player
    Send(
        reinterpret_cast<char*>(returnBuffer),
        SerializationHeaders::PlayResult, sizeof(int) * 3,
        static_cast<PacketSendTarget>(clientNumber)
    ); 

    // send to the other player
    returnBuffer[1] = ~returnBuffer[1]; // basically !isOtherPlayerTurn
    Send(
        reinterpret_cast<char*>(returnBuffer),
        SerializationHeaders::PlayResult, sizeof(int) * 3,
        // (PacketSendTarget)(int)!(bool)clientNumber // nasty but would ve made me tremendously happy
        clientNumber == 0 ? PacketSendTarget::Client1 : PacketSendTarget::Client0
    );
}

void ServerSocket::HandleDisconnection(SOCKET& clientSocket, const std::string& name, const int clientNumber)
{
    std::cout << name << " disconnected" << std::endl;

    std::string logoutLog = "[Server]: " + name + " disconnected";
    GameServer::StoreChatMessage(logoutLog);
    Send(
        logoutLog.c_str(),
        SerializationHeaders::ChatMessage,
        static_cast<int>(logoutLog.size()) + 1,
        clientNumber == 0 ? PacketSendTarget::Client1 : PacketSendTarget::Client0
    );
}

