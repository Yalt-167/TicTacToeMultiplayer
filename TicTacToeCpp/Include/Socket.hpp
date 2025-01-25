#pragma once


class Socket
{
public:
    ~Socket() { }
    Socket()
    {
        InitWSA();

        socket_ = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_ == INVALID_SOCKET)
        {
            std::cerr << "Couldn create Socket" << std::endl;
            WSACleanup();
            throw std::exception("Couldn create Socket");
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
    }
protected:
    void InitWSA()
    {
        WORD version = MAKEWORD(2, 2); // request the 2.2 version of Winsock 

        if (WSAStartup(version, &wsaData) != 0)
        {
            std::cerr << "Couldn start WSA" << std::endl;
            throw std::exception("Couldn start WSA");
        }
    }

    void Cleanup() const
    {
        closesocket(socket_);

        WSACleanup();
    }
public:
    virtual void Run() = 0;
    void Send(const char* data, SerializationHeaders what)
    {
        Send(data, what, (int)strlen(data));
    }

    void Send(const char* data, SerializationHeaders what, int size)
    {
        memcpy(headerBuffer, &header.Set(what, size), sizeof(Header));
        _ = send(socket_, headerBuffer, sizeof(Header), 0);

        _ = send(socket_, data, size, 0);
    }

protected:
    SOCKET socket_;
    WSADATA wsaData; // contains infos about WIndows sockets impl
    sockaddr_in serverAddr = sockaddr_in(); // for specifying IPv4 socket addresses && protocols

    Header header;
    char headerBuffer[sizeof(Header)];
};