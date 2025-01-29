#pragma once 

#include "Socket.hpp"


class ServerSocket : public Socket
{
public:
    ~ServerSocket() {}
    ServerSocket() : Socket()
    {
        Init();
        clientThreads.resize(2);
        clientSockets.resize(2);
    }
    void Run() override
    {
        while (true)
        {
            if (connectedClients < 2) {
                sockaddr_in clientAddr;
                int clientSize = sizeof(clientAddr);
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
                _ = send(clientSocket, reinterpret_cast<char*>(&(connectedClients)), sizeof(int), 0);

                clientSockets[connectedClients] = clientSocket;
                clientThreads[connectedClients] = std::thread(
                    [=]()
                    {
                        HandleClient(clientSocket, nameBuffer, connectedClients++);
                    }
                );

                std::cout << nameBuffer << " connected" << std::endl;
            }
        }

        for (int i = 0; i < clientThreads.size(); i++)
        {
            if (clientThreads[i].joinable())
            {
                clientThreads[i].join();
            }
        }

        Cleanup();
    }

    int GetConnectedClientsCount() const
    {
        return connectedClients;
    }
    
private:
    void Init()
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


    std::vector<std::thread> clientThreads;
    std::vector<SOCKET> clientSockets;
    int connectedClients = 0;

    void HandleClient(SOCKET clientSocket, const char* name, int clientNumber)
    {
        char headerBuffer[sizeof(PacketHeader)];

        while (true)
        {
            if (recv(clientSocket, headerBuffer, sizeof(PacketHeader), 0) <= 0)
            {
                std::cout << name << " disconnected" << std::endl;
                connectedClients--;

                break;
            }

            int* header_ = reinterpret_cast<int*>(headerBuffer);
            std::cerr << "SerializationHeader" << header_[0] << std::endl;
            switch ((SerializationHeaders)header_[0])
            {
                case SerializationHeaders::Login:
                    std::cout << "Got login packet from " << clientNumber << std::endl;
                    break;

                case SerializationHeaders::ChatMessage:
                    std::cout << "Got chat packet from " << clientNumber << std::endl;
                    HandleChatMessage(clientSocket, name, clientNumber, header_[1]);
                    break;

                case SerializationHeaders::Play:
                    HandlePlay(clientSocket, clientNumber);
                    break;
            }

        }

        delete[] name;

        _ = closesocket(clientSocket);
    }

    void HandleChatMessage(SOCKET& clientSocket, const char* name, int clientNumber, int messageSize)
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

    void HandlePlay(SOCKET& clientSocket, int clientNumber)
    {
        char playBuffer[sizeof(int)];

        _ = recv(clientSocket, playBuffer, sizeof(int), 0);

        int playValidity = (int)(PlayIsValid(*reinterpret_cast<int*>(playBuffer)) ? Plays::InvalidPlay : Plays::ValidPlay);

        int otherClientIndex = (int)(!(bool)clientNumber);
        _ = send(
            clientSockets[otherClientIndex],
            reinterpret_cast<char*>(&header.Set(SerializationHeaders::Play, sizeof(int))),
            sizeof(PacketHeader),
            0
        );

        _ = send(clientSockets[otherClientIndex], reinterpret_cast<char*>(&playValidity), sizeof(int), 0);
    }

    bool PlayIsValid(int play)
    {
        return false;
    }
};
