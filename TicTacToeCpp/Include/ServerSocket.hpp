#pragma once 

#include "Socket.hpp"


class ServerSocket : public Socket
{
public:
    ServerSocket();
    ~ServerSocket();
    void Run() override;

    int GetConnectedClientsCount() const;

    void Send(const char* data, SerializationHeaders what, int size, PacketSendTarget target);
private:
    void Init();


    std::vector<std::thread> clientThreads;
    std::vector<SOCKET> clientSockets;
    int connectedClients = 0;

    void HandleClient(SOCKET clientSocket, const char* name, int clientNumber);

    void HandleChatMessage(SOCKET& clientSocket, const char* name, int clientNumber, int messageSize);

    void HandlePlay(SOCKET& clientSocket, int clientNumber);

    bool TryPlay(int play, int clientNumber);
};