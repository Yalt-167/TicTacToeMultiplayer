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
        chatWriterThread.join();

        Cleanup();
    }
private:
    void Init()
    {
        //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost address -> basically connect to self // lowkey lonely behaviour

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
            std::cerr << "Couldn t connect to server" << std::endl;
            Cleanup();
            throw std::exception("Couldn t connect to server");
        }

        int nameSize = (int)userName.size() + 1; // include null terminator
        _ = send(socket_, reinterpret_cast<char*>(&header.Set(SerializationHeaders::ConnectionEvent, nameSize)), sizeof(PacketHeader), 0);

        _ = send(socket_, userName.c_str(), nameSize, 0);

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

        chatWriterThread = std::thread(
            [this]()
            {
                WriteToChat();
            }
        );
    }

    void Send(const char* data, SerializationHeaders what, int size)
    {
        _ = send(socket_, reinterpret_cast<char*>(&header.Set(what, size)), sizeof(PacketHeader), 0);

        _ = send(socket_, data, size, 0);
    }
private:
    void WriteToChat()
    {
        std::cin.ignore(); // clears cin s buffer

        char userInput[CHAT_BUFFER_SIZE];
        userInput[CHAT_BUFFER_SIZE - 1] = '\0';

        PacketHeader header { (int)SerializationHeaders::ChatMessage, 0 };
        
        while (true)
        {
            std::cout << userName << ": ";
            std::cin.getline(userInput, CHAT_BUFFER_SIZE);

            if (strcmp(userInput, ":q") == 0) { break; } // ^^ we be WIMing

            int messageSize = (int)strlen(userInput) + 1;
            
            header.Size = messageSize;
            _ = send(socket_, reinterpret_cast<char*>(&header), sizeof(PacketHeader), 0);

            _ = send(socket_, userInput, messageSize, 0);
        }
    }

    void Listen()
    {
        char header[sizeof(PacketHeader)];
        while (true)
        {
            if (recv(socket_, header, sizeof(PacketHeader), 0) > 0) // only receive the header
            {
                int* header_ = reinterpret_cast<int*>(header);
                std::cerr << "\rPacketSize" << header_[1] << std::endl;
                std::cerr << "\rSerializationHeader" << header_[0] << std::endl;
                switch ((SerializationHeaders)header_[0])
                {
                    case SerializationHeaders::ChatMessage:
                        std::cout << "\rReceived a Chat" << std::endl;
                        HandleChatMessage(header_[1]);
                        break;
                    

                    case SerializationHeaders::Play:
                        std::cerr << "\rShouln t have received SerializationHeaders::Play" << std::endl;
                        throw std::exception("^^");
                        break;

                    case SerializationHeaders::PlayResult:
                        std::cout << "\rReceived a PlayResult" << std::endl;
                        HandlePlay();
                        break;
                    
                    default:
                        std::cerr << "\rInvalid SerializationHeader" << header_[0] << std::endl;
                        break;
                }
            } 

            std::cout << userName << ": ";
        }
    }

    void HandleChatMessage(int bufferSize)
    {
        char* chatBuffer = new char[bufferSize];

        _ = recv(socket_, chatBuffer, bufferSize, 0);

        std::cout << "\r" << chatBuffer << std::endl;
        std::cout << userName << ": ";
    }

    void HandlePlay()
    {
        char play[sizeof(int) * 3];

        _ = recv(socket_, play, sizeof(int) * 2, 0);

        int* plays = reinterpret_cast<int*>(&play);

        switch ((GameResult)plays[0])
        {
            case GameResult::PlayerOneWon:
                std::cout << "\r" << "Player 1 won" << std::endl;
                break;

            case GameResult::Draw:
                std::cout << "\r" << "Draw" << std::endl;
                break;

            case GameResult::PlayerTwoWon:
                std::cout << "\r" << "Player 2 won" << std::endl;
                break;

            case GameResult::None:
            default:
                break;
        }

        if ((Plays)plays[1] == Plays::InvalidPlay)
        {
            std::cout << "InvalidPlay" << std::endl;
        }
        else
        {
            std::cout << "Someone played: " << plays[1] << std::endl;
            Grid::Place(plays[1], 1);
        }

        std::cout << userName << ": ";
        
    }

    std::string userName;
    std::thread listenerThread;
    std::thread chatWriterThread;
};
