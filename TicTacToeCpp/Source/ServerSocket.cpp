#include "ServerSocket.hpp"
#include "GameServer.hpp"


ServerSocket::ServerSocket() : Socket()
{
    Init();
    clientThreads.resize(2);
    clientSockets.resize(2);
}
ServerSocket::~ServerSocket() { }

void ServerSocket::Run()
{
    while (true)
    {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(socket_, (sockaddr*)&clientAddr, &clientSize);

        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Failed to accept client" << std::endl;
            continue;
        }

        if (recv(clientSocket, headerBuffer, sizeof(PacketHeader), 0) <= 0)
        {
            std::cerr << "Gone too soon" << std::endl;
            continue;
        }

        int nameSize = reinterpret_cast<int*>(headerBuffer)[1] + 1; // include null terminator
        char* nameBuffer = new char[nameSize];

        _ = recv(clientSocket, nameBuffer, nameSize, 0);

        clientSockets[connectedClients] = clientSocket;
        clientThreads[connectedClients] = std::thread(
            [=]()
            {
                HandleClient(clientSocket, nameBuffer, connectedClients++);
            }
        );

        std::cout << nameBuffer << " connected" << std::endl;
    }

    for (int i = 0; i < clientThreads.size(); i++)
    {
        if (clientThreads[i].joinable())
        {
            clientThreads[i].join();
        }
    }

    Cleanup();
}

int ServerSocket::GetConnectedClientsCount() const
{
    return connectedClients;
}

void ServerSocket::Send(const char* data, SerializationHeaders what, int size, PacketSendTarget target)
{
    if (target == PacketSendTarget::Broadcast)
    {
        for (int i = 0; i < clientSockets.size(); i++)
        {
            _ = send(clientSockets[i], reinterpret_cast<char*>(&header.Set(what, size)), sizeof(PacketHeader), 0);

            _ = send(clientSockets[i], data, size, 0);
        }
    }
    else
    {
        _ = send(clientSockets[(int)target], reinterpret_cast<char*>(&header.Set(what, size)), sizeof(PacketHeader), 0);

        _ = send(clientSockets[(int)target], data, size, 0);
    }
}

void ServerSocket::Init()
{
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) // setting up as server
    {
        std::cerr << "Bind failed" << std::endl;
        Cleanup();

        throw std::exception("Bind failed");
    }

    if (listen(socket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        Cleanup();

        throw std::exception("Listen failed");
    }

    std::cout << "Server should be listening on port " << PORT << "..." << std::endl;
}

void ServerSocket::HandleClient(SOCKET clientSocket, const char* name, int clientNumber)
{
    char headerBuffer[sizeof(PacketHeader)];

    while (true)
    {
        if (recv(clientSocket, headerBuffer, sizeof(PacketHeader), 0) <= 0)
        {
            std::cout << name << " disconnected" << std::endl;
            connectedClients--;

            break;
        }

        int* header_ = reinterpret_cast<int*>(headerBuffer);
        std::cerr << "SerializationHeader" << header_[0] << " | Size: " << header_[1] << std::endl;
        switch ((SerializationHeaders)header_[0])
        {
        case SerializationHeaders::ConnectionEvent:
            std::cout << "Got ConnectionEvent packet from " << clientNumber << std::endl;
            break;

        case SerializationHeaders::ChatMessage:
            std::cout << "Got Chat packet from " << clientNumber << std::endl;
            HandleChatMessage(clientSocket, name, clientNumber, header_[1]);
            break;

        case SerializationHeaders::Play:
            std::cout << "Got Play packet from " << clientNumber << std::endl;
            HandlePlay(clientSocket, clientNumber);
            break;
        }

    }

    delete[] name;

    _ = closesocket(clientSocket);
}

void ServerSocket::HandleChatMessage(SOCKET& clientSocket, const char* name, int clientNumber, int messageSize)
{
    int nameLength = (int)strlen(name);
    int totalSize = nameLength + messageSize + 3; // + 3 -> account for ": " + null terminator
    char* message = new char[totalSize];
    char* chatBuffer = new char[messageSize];

    _ = recv(clientSocket, chatBuffer, messageSize, 0);

    std::cout << name << ": " << chatBuffer << std::endl;

    if (connectedClients < 2)
    {
        std::cout << "Message not sent because there is only one client connected" << std::endl;
        delete[] message;
        delete[] chatBuffer;
        return; // why even bother you are lonely AF just give up
    }

    _ = strcpy_s(message, totalSize, name);
    _ = strcpy_s(message + nameLength, totalSize, ": ");
    _ = strcpy_s(message + nameLength + 2, totalSize, chatBuffer);
    message[totalSize - 1] = '\0';

    int otherClientIndex = (int)(!(bool)clientNumber); // lowkey sinning
    _ = send(
        clientSockets[otherClientIndex],
        reinterpret_cast<char*>(&header.Set(SerializationHeaders::ChatMessage, totalSize)),
        sizeof(PacketHeader),
        0
    );

    _ = send(clientSockets[otherClientIndex], message, totalSize, 0);

    delete[] chatBuffer;
    //delete[] message // fails for some reason
}

void ServerSocket::HandlePlay(SOCKET& clientSocket, int clientNumber)
{
    char playBuffer[sizeof(int)];

    _ = recv(clientSocket, playBuffer, sizeof(int), 0);

    int returnBuffer[4]; // wether the game state has changed + the play itself + wether u can play + who played

    int playRequest = *reinterpret_cast<int*>(playBuffer);

    returnBuffer[3] = (int)!(bool)GameServer::Instance->PlayerTurn; // lowkey despise implicit casting
    bool validPlay = TryPlay(playRequest, clientNumber);


    if (validPlay)
    {
        if (Grid::CheckWin())
        {
            returnBuffer[0] = ((int)GameResult::PlayerOneWon + clientNumber);
            // bc both win result are in a row and <clientNumber> is 0 or 1 so the addition corrects the statement
            // im dogshit at explaining things but I swear it makes sense
        }
        else if (Grid::CheckDraw())
        {
            returnBuffer[0] = (int)GameResult::Draw;
        }
        else
        {
            returnBuffer[0] = (int)GameResult::None;
        }

        returnBuffer[1] = playRequest;
        returnBuffer[2] = false; // lowkey despise implicit casting
    }
    else
    {
        returnBuffer[0] = (int)GameResult::None;
        returnBuffer[1] = (int)Plays::InvalidPlay;
        returnBuffer[2] = true; // lowkey hate implicit casting
    }


    // send back to the player
    Send(
        reinterpret_cast<char*>(returnBuffer),
        SerializationHeaders::PlayResult, sizeof(int) * 4,
        (PacketSendTarget)clientNumber
    );

    // send to the other player
    returnBuffer[2] = ~returnBuffer[2]; // basically !isOtherPlayerTurn
    Send(
        reinterpret_cast<char*>(returnBuffer),
        SerializationHeaders::PlayResult, sizeof(int) * 4,
        (PacketSendTarget)(int)!(bool)clientNumber // nasty but makes me tremendously happy // (IK it could ve been (PacketSendTarget)!clientNumber )
    );
}

bool ServerSocket::TryPlay(int play, int clientNumber)
{
    if (clientNumber != GameServer::Instance->PlayerTurn) { return false; }

    if (!Grid::IsSlotEmpty(play)) { return false; }

    GameServer::Instance->PlayerTurn = (int)!(bool)GameServer::Instance->PlayerTurn;
    Grid::Place(play, (bool)clientNumber);

    return true;
}
