#pragma once 

#include "Socket.hpp" 
#include "Grid.hpp"


class ClientSocket : public Socket
{
public:
    ClientSocket(const std::string& userName_, std::vector<std::string>* chatMessages_);
    ~ClientSocket();
private:
    void Init();
public:
    void Run() override;

    void Send(const char* data, const SerializationHeaders what, const  int size);
private:
    void WriteToChat();
    void Listen();

    void HandleChatMessage(const int bufferSize);
    void HandlePlayResult();
    void HandleCatchupPacket();

    std::string userName;
    std::vector<std::string>* chatMessages;
    std::thread* listenerThread;
    std::thread* chatWriterThread;
};
