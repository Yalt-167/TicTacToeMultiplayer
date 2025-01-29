#pragma once 

#include "Socket.hpp" 


class GameClient;


class ClientSocket : public Socket
{
public:
    ClientSocket(const std::string& userName_, GameClient* game_);
    ~ClientSocket();

private:
    void Init();

public:
    void Run() override;
    int ClientIndex() const;

private:
    void WriteToChat();
    void Listen();
    void HandleChatMessage(int bufferSize);
    void HandlePlay();

    std::string userName;
    std::thread listenerThread;
    std::thread chatWriterThread;

    GameClient* game;
    int clientIndex;
};
