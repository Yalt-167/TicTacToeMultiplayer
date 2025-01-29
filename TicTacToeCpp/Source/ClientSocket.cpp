#include "ClientSocket.hpp"

#include "GameClient.hpp"


ClientSocket::ClientSocket(const std::string& userName_, GameClient* game_) : Socket()
{
    userName = userName_;
    game = game_;

    Init();
}

ClientSocket::~ClientSocket()
{
    listenerThread.join();
    chatWriterThread.join();

    Cleanup();
}

void ClientSocket::Init()
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
    _ = send(socket_, reinterpret_cast<char*>(&header.Set(SerializationHeaders::Login, nameSize)), sizeof(PacketHeader), 0);

    _ = send(socket_, userName.c_str(), nameSize, 0);

    char tempBuffer[sizeof(int)]; // temporary buffer for the reception of the client index
    _ = recv(socket_, tempBuffer, sizeof(int), 0);
    clientIndex = std::memcmp(&clientIndex, tempBuffer, sizeof(int));

    std::cout << "Should be connected to the server ^^" << std::endl;
}

void ClientSocket::Run()
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

int ClientSocket::ClientIndex() const {
    return clientIndex;
}

void ClientSocket::WriteToChat()
{
    std::cin.ignore(); // clears cin s buffer

    char userInput[CHAT_BUFFER_SIZE];
    userInput[CHAT_BUFFER_SIZE - 1] = '\0';

    PacketHeader header{ (int)SerializationHeaders::ChatMessage, 0 };

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

void ClientSocket::Listen()
{
    char header[sizeof(PacketHeader)];
    while (true)
    {
        if (recv(socket_, header, sizeof(PacketHeader), 0) > 0) // only receive the header
        {
            int* header_ = reinterpret_cast<int*>(header);
            std::cerr << "SerializationHeader" << header_[0] << std::endl;
            switch ((SerializationHeaders)header_[0])
            {
            case SerializationHeaders::ChatMessage:
                HandleChatMessage(header_[1]);
                break;


            case SerializationHeaders::Play:
                HandlePlay();
                break;

            default:
                std::cerr << "Invalid SerializationHeader" << header_[0] << std::endl;
                break;
            }
        }
    }
}

void ClientSocket::HandleChatMessage(int bufferSize)
{
    char* chatBuffer = new char[bufferSize];

    _ = recv(socket_, chatBuffer, bufferSize, 0);

    std::cout << "\r" << chatBuffer << std::endl;
    std::cout << userName << ": ";
}

void ClientSocket::HandlePlay()
{
    char playBuffer[sizeof(int)];

    _ = recv(socket_, playBuffer, sizeof(int), 0);

    int play = reinterpret_cast<int>(playBuffer);

    switch ((Plays)play)
    {
    case Plays::InvalidPlay:
        std::cout << "\r" << "InvalidPlay" << std::endl;
        std::cout << userName << ": ";
        break;

    case Plays::ValidPlay:
        std::cout << "\r" << "Nice" << std::endl;
        std::cout << userName << ": ";
        break;

    case Plays::PlayerOneWon:
        std::cout << "\r" << "Player 1 won" << std::endl;
        std::cout << userName << ": ";
        break;

    case Plays::Draw:
        std::cout << "\r" << "Draw" << std::endl;
        std::cout << userName << ": ";
        break;

    case Plays::PlayerTwoWon:
        std::cout << "\r" << "Player 2 won" << std::endl;
        std::cout << userName << ": ";
        break;

    default:
        std::cout << "\r" << "Someone played: " << play << std::endl;
        std::cout << userName << ": ";
        game->PlaceOnGrid(play, clientIndex);
        game->AllowPlay();
        break;
    }
}