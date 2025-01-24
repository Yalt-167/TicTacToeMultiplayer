#pragma once 

#include "Socket.hpp" 


class ClientSocket : public Socket
{
public:
    ClientSocket(const std::string& userName_) : Socket()
    {
        userName = userName_;

        Init();
    }
    ~ClientSocket()
    {
        listenerThread.join();
        writerThread.join();

        Cleanup();
    }
private:
    void Init()
    {
        //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // loopback address -> basically connect to self // lowkey lonely behaviour

        switch (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr))
        {
        case 0:
            std::cerr << "You fucked up" << std::endl;
            std::cerr << "Incorrect input to inet_pton" << std::endl;
            throw std::exception("Invalid address");
            break;

        case -1:
            std::cerr << "it just failed" << std::endl;
            _ = WSAGetLastError();
            throw std::exception("Invalid address");
            break;

        default:
            break;
        }

        if (connect(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) // connecting to server
        {
            std::cerr << "Couldn connect to server" << std::endl;
            Cleanup();
            throw std::exception("Couldn connect to server");
        }

        _  = send(socket_, userName.c_str(), (int)userName.size(), 0);
        std::cout << "Should be connected to the server ^^" << std::endl;
    }
public:
    void Run() override
    {
        listenerThread = std::thread(
            [this]()
            {
                Listen();
            }
        );

        writerThread = std::thread(
            [this]()
            {
                Write();
            }
        );
    }

    void Write()
    {
        std::cin.ignore(); // clears cin s buffer

        char userInput[BUFFER_SIZE];

        while (true)
        {
            
            std::cout << userName << ": ";
            std::cin.getline(userInput, BUFFER_SIZE);

            if (strcmp(userInput, ":q") == 0) { break; } // ^^ we be WIMing

            _ = send(socket_, userInput, BUFFER_SIZE, 0);
        }
    }

    void Listen()
    {
        char dataBuffer[BUFFER_SIZE];

        while (true)
        {
            if (recv(socket_, dataBuffer, BUFFER_SIZE, 0) > 0)
            {
                std::cout << "\r" << dataBuffer << std::endl;
                std::cout << userName << ": ";
            }
        }
    }
private:
    std::string userName;
    std::thread listenerThread;
    std::thread writerThread;
};
