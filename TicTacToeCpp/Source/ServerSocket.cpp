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

        connectedClients++;

        clientSockets[clientIndex] = clientSocket;
        clientThreads[clientIndex] = std::thread(
            [=]()
            {
                HandleClient(clientSocket, nameBuffer, clientIndex);
            }
        );
        clientThreads[clientIndex].detach();

        std::cout << nameBuffer << " connected" << std::endl;
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
        std::cout
            << "Got " << PacketHeader::LegibleSerializationHeaders(header.SerializationHeader)
            << " packet from [" << name 
            << "] || Size: " << header.Size
            << std::endl;
        switch (header.SerializationHeader)
        {
        case SerializationHeaders::Disconnection:
            HandleDisconnection(clientSocket, name, clientNumber);
            socketOpen = false;
            break;

        case SerializationHeaders::ChatMessage:
            HandleChatMessage(clientSocket, name, clientNumber, header.Size);
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

void ServerSocket::HandleChatMessage(SOCKET& clientSocket, const char* name, const int clientNumber, const int messageSize)
{
    int nameLength = (int)strlen(name);
    int totalSize = nameLength + messageSize + 3; // + 3 -> account for ": " + null terminator
    char* message = new char[totalSize];
    char* chatBuffer = new char[messageSize];

    _ = recv(clientSocket, chatBuffer, messageSize, 0);

    std::cout << name << ": " << chatBuffer << std::endl;

    if (connectedClients < 2)
    {
        std::cout << "Message not sent because there is only one client connected" << std::endl;
        delete[] message;
        delete[] chatBuffer;
        return; // why even bother you are lonely AF just give up
    }

    _ = strcpy_s(message, totalSize, name);
    _ = strcpy_s(message + nameLength, totalSize, ": ");
    _ = strcpy_s(message + nameLength + 2, totalSize, chatBuffer);
    message[totalSize - 1] = '\0';

    //int otherClientIndex = (int)(!(bool)clientNumber); // lowkey sinning // i was forced into the "righteous" path
    int otherClientIndex = clientNumber == 0 ? 1 : 0;
    _ = send(clientSockets[otherClientIndex], header.Set(SerializationHeaders::ChatMessage, totalSize), sizeof(PacketHeader), 0);

    _ = send(clientSockets[otherClientIndex], message, totalSize, 0);

    delete[] chatBuffer;
    //delete[] message // fails for some reason
}

void ServerSocket::HandlePlay(SOCKET& clientSocket, const int clientNumber)
{
    char playBuffer[sizeof(int)];

    _ = recv(clientSocket, playBuffer, sizeof(int), 0);

    int returnBuffer[4]; // { <wether the game state has changed>, <the play itself>, <wether u can play>, <who played> }

    GameServer::ParsePlay(*reinterpret_cast<int*>(playBuffer), returnBuffer, clientNumber);

    // send back to the player
    Send(
        reinterpret_cast<char*>(returnBuffer),
        SerializationHeaders::PlayResult, sizeof(int) * 4,
        (PacketSendTarget)clientNumber
    );

    // send to the other player
    returnBuffer[2] = ~returnBuffer[2]; // basically !isOtherPlayerTurn
    Send(
        reinterpret_cast<char*>(returnBuffer),
        SerializationHeaders::PlayResult, sizeof(int) * 4,
        // (PacketSendTarget)(int)!(bool)clientNumber // nasty but makes me tremendously happy // (IK it could ve been (PacketSendTarget)!clientNumber )
        clientNumber == 0 ? PacketSendTarget::Client1 : PacketSendTarget::Client0
    );
}

void ServerSocket::HandleDisconnection(SOCKET& clientSocket, const std::string& name, const int clientNumber)
{
        std::string logoutLog = "[Server]: " + name + " disconnected";
        Send(
            logoutLog.c_str(),
            SerializationHeaders::ChatMessage, static_cast<int>(logoutLog.size()) + 1,
            clientNumber == 0 ? PacketSendTarget::Client1 : PacketSendTarget::Client0
        );
}

