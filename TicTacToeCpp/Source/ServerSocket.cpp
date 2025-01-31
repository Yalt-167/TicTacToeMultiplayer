#include "ServerSocket.hpp"
#include "GameServer.hpp"


ServerSocket::ServerSocket() : Socket()
{
    Init();
    clientThreads.resize(2);
    clientSockets.resize(2);
}
ServerSocket::~ServerSocket() { }

void ServerSocket::Run()
{
    sockaddr_in clientAddr;
    int clientSize = sizeof(clientAddr);
    while (true)
    {
        if (connectedClients == 2) { continue; }

        SOCKET clientSocket = accept(socket_, (sockaddr*)&clientAddr, &clientSize);
        
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

    Cleanup();
}

int ServerSocket::GetConnectedClientsCount() const
{
    return connectedClients;
}

void ServerSocket::Send(const char* data, SerializationHeaders what, const int size, PacketSendTarget target)
{
    if (target == PacketSendTarget::Broadcast)
    {
        for (int i = 0; i < clientSockets.size(); i++)
        {
            _ = send(clientSockets[i], reinterpret_cast<char*>(&header.Set(what, size)), sizeof(PacketHeader), 0);

            _ = send(clientSockets[i], data, size, 0);
        }
    }
    else
    {
        _ = send(clientSockets[(int)target], reinterpret_cast<char*>(&header.Set(what, size)), sizeof(PacketHeader), 0);

        _ = send(clientSockets[(int)target], data, size, 0);
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
            std::cout << name << " disconnected" << std::endl;
            break;
        }

        int* header_ = reinterpret_cast<int*>(headerBuffer);
        SerializationHeaders serializationHeader = (SerializationHeaders)header_[0];
        std::cout
            << "Got " <<
            //LegibleSerializationHeaders(
                (int)
            serializationHeader
            //) 
            << " packet from "
            << name 
            << " || Size: " << header_[1]
            << std::endl;
        switch (serializationHeader)
        {
        case SerializationHeaders::ConnectionEvent:
            socketOpen = HandleConnectionEvent(clientSocket, name, clientNumber);
            break;

        case SerializationHeaders::ChatMessage:
            HandleChatMessage(clientSocket, name, clientNumber, header_[1]);
            break;

        case SerializationHeaders::Play:
            HandlePlay(clientSocket, clientNumber);
            break;
        }
    }
    
    delete[] name;
    
    clientThreads.erase(clientThreads.begin() + clientNumber);
    clientSockets.erase(clientSockets.begin() + clientNumber);

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

    int otherClientIndex = (int)(!(bool)clientNumber); // lowkey sinning
    _ = send(
        clientSockets[otherClientIndex],
        reinterpret_cast<char*>(&header.Set(SerializationHeaders::ChatMessage, totalSize)),
        sizeof(PacketHeader),
        0
    );

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

bool ServerSocket::HandleConnectionEvent(SOCKET& clientSocket, const std::string& name, const int clientNumber)
{
    char eventDataBuffer[sizeof(int)];

    _ = recv(clientSocket, eventDataBuffer, sizeof(int), 0);

    if (*reinterpret_cast<int*>(eventDataBuffer) < 0)
    {
        std::string logoutLog = "[Server]: " + name + " disconnected";
        Send(
            logoutLog.c_str(),
            SerializationHeaders::ChatMessage, logoutLog.size() + 1,
            clientNumber == 0 ? PacketSendTarget::Client1 : PacketSendTarget::Client0
        );
        return false;
    }
    else
    {
        return true;
    }
}

