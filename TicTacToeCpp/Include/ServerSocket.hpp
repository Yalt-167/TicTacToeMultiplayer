#pragma once 

#include "Socket.hpp"
#include "PacketSendTarget.hpp"


class ServerSocket : public Socket
{
public:
    ServerSocket();
    ~ServerSocket();
    void Run() override;

    int GetConnectedClientsCount() const;

    void Send(const char* data, const SerializationHeaders what, const int size, const PacketSendTarget target);
private:
    void Init();


    std::unordered_map<int, std::thread> clientThreads;
    std::unordered_map<int, SOCKET> clientSockets;
    int connectedClients = 0;
    std::vector<int> freeClientIDs{ 0, 1 };

    void HandleClient(SOCKET clientSocket, const char* name, const int clientNumber);

    void HandleChatMessage(SOCKET& clientSocket, const char* name, const int clientNumber, const int messageSize);
    void HandlePlay(SOCKET& clientSocket, const int clientNumber);
    void HandleDisconnection(SOCKET& clientSocket, const std::string& name, const int clientNumber);
};