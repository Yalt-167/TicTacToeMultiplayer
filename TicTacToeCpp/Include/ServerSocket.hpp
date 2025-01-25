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
        char buffer[CHAT_BUFFER_SIZE];

        while (true)
        {
            sockaddr_in clientAddr;
            int clientSize = sizeof(clientAddr);
            SOCKET clientSocket = accept(socket_, (sockaddr*)&clientAddr, &clientSize);

            if (clientSocket == INVALID_SOCKET)
            {
                std::cerr << "Failed to accept client" << std::endl;
                continue;
            }

            if (recv(clientSocket, buffer, CHAT_BUFFER_SIZE, 0) <= 0)
            {
                std::cerr << "Gone too soon" << std::endl;
                continue;
            }


            clientSockets[connectedClients] = clientSocket;
            clientThreads[connectedClients] = std::thread(
                [=]()
                {
                    HandleClient(clientSocket, buffer, connectedClients++); // hopefully this ++ doesn t cause concurrency issues
                }
            );

            std::cout << buffer << " connected." << std::endl;
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

    void HandleClient(SOCKET clientSocket, const std::string& name, int clientNumber)
    {
        char headerBuffer[sizeof(Header)];

        int nameLength = (int)name.length() + 3; // + 3 ->account for :<space> + null terminator
        while (true)
        {
            int bytesReceived = recv(clientSocket, headerBuffer, sizeof(Header), 0);
            if (bytesReceived <= 0)
            {
                std::cout << name  << " disconnected" << std::endl;
                connectedClients--;

                break;
            }

            switch ((SerializationHeaders)headerBuffer[0])
            {
                case SerializationHeaders::ChatMessage:
                    HandleChatMessage(clientSocket, name, clientNumber);
                    break;

                case SerializationHeaders::Play:
                    HandlePlay(clientSocket, clientNumber);
                    break;
            }

        }

        _ = closesocket(clientSocket);
    }

    void HandleChatMessage(SOCKET& clientSocket, const std::string& name, int clientNumber)
    {
        char message[CHAT_BUFFER_SIZE];
        char chatBuffer[CHAT_BUFFER_SIZE];

        _ = recv(clientSocket, chatBuffer, CHAT_BUFFER_SIZE, 0);

        std::cout << name << ": " << chatBuffer << std::endl;

        _ = strcpy_s(message, name.c_str());
        _ = strcat_s(message, ": ");
        _ = strcat_s(message, chatBuffer);

        if (connectedClients < 2)
        {
            return; // why even bother you are lonely AF just give up
        }

        //if (bytesReceived + nameLength > CHAT_BUFFER_SIZE)
        //{
        //    // account for this at some point
        //}

        _ = send(clientSockets[(int)(!(bool)clientNumber)], message, CHAT_BUFFER_SIZE, 0); // lowkey sinning
    }

    void HandlePlay(SOCKET& clientSocket, int clientNumber)
    {
        char playBuffer[sizeof(int)];

        _ = recv(clientSocket, playBuffer, sizeof(int), 0);

        int playValidity = PlayIsValid(*reinterpret_cast<int*>(playBuffer)) ? 9 : 10;

        memcpy(playBuffer, &playValidity, sizeof(int));
        Send(playBuffer, SerializationHeaders::Play);
    }

    bool PlayIsValid(int play)
    {
        return false;
    }
};
