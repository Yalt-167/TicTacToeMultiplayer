#pragma once 

#include "Socket.hpp"


class ServerSocket : public Socket
{
public:
    ServerSocket();
    ~ServerSocket();
    void Run() override;

    int GetConnectedClientsCount() const;

    void Send(const char* data, SerializationHeaders what, const int size, enum class PacketSendTarget target);
private:
    void Init();


    std::vector<std::thread> clientThreads;
    std::vector<SOCKET> clientSockets;
    int connectedClients = 0;
    std::vector<int> freeClientIDs{ 0, 1 };

    void HandleClient(SOCKET clientSocket, const char* name, const int clientNumber);

    void HandleChatMessage(SOCKET& clientSocket, const char* name, const int clientNumber, const int messageSize);
    void HandlePlay(SOCKET& clientSocket, const int clientNumber);
    bool HandleConnectionEvent(SOCKET& clientSocket, const std::string& name, const int clientNumber);
};