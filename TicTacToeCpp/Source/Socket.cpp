#include "Socket.hpp"


Socket::Socket() {
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

void Socket::InitWSA()
{
    WORD version = MAKEWORD(2, 2); // request the 2.2 version of Winsock 

    if (WSAStartup(version, &wsaData) != 0)
    {
        std::cerr << "Couldn start WSA" << std::endl;
        throw std::exception("Couldn start WSA");
    }
}

void Socket::Cleanup() const
{
    closesocket(socket_);

    WSACleanup();
}

void Socket::Send(const char* data, SerializationHeaders what)
{
    Send(data, what, (int)strlen(data));
}

void Socket::Send(const char* data, SerializationHeaders what, int size)
{
    _ = send(socket_, reinterpret_cast<char*>(&header.Set(what, size)), sizeof(PacketHeader), 0);

    _ = send(socket_, data, size, 0);
}