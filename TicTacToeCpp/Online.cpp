#include "Online.hpp"




bool SocketController::initWinSock2() {

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }

    return true;
}


SOCKET SocketController::createSocket(int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // Port à écouter.
    linkSockets.push_back(sock);
    toCheckSockets.push_back(sock);
    return sock;
}


bool SocketController::Bind(SOCKET& sock) {
    
    
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accepte toutes les connexions entrantes.

    if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }
    return true;
}


bool SocketController::Connect(SOCKET& sock, int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Adresse IP du serveur.

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }
    return true;
}

bool SocketController::Listen(SOCKET& sock) {
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }
    return true;
}

bool SocketController::Accept(SOCKET& clientSocket, SOCKET& serverSocket) {
    clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Client Connected" << std::endl;
    
    return true;
}

bool SocketController::Send(SOCKET& sock, std::string username, std::string message) {
    if (message == "")
    {
        std::cout << (username.empty() ? "" : username + " : ");
        std::getline(std::cin, message);
        typing = false;
    }

    // Construire le message en fonction de la présence d'un username
    std::string fullMessage = username.empty() ? message : (username + " : " + message);

    int bytesSent = send(sock, fullMessage.c_str(), fullMessage.size(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << "Message sent" << std::endl;
    return true;
}

char* SocketController::receive(SOCKET& sock, std::string message)
{
    int bufferSize = 512;
    char* buffer = new char[bufferSize];
    while (true)
    {
        int bytesReceived = recv(sock, buffer, bufferSize - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Null-terminate le message reçu.
            std::cout << "in rcv" << std::endl;
            std::cout << buffer << std::endl;
            isListening = false;
            message = buffer;
            toCheckSockets.push_back(sock);
            break;
            return buffer;
        }
        else if (bytesReceived == 0) {
            std::cout << "Connection closed by peer." << std::endl;
        }

    }
    
}

void SocketController::speak(SOCKET& serverSocket, std::string username) {
    while (true)
    {
        if (!typing) {
            Threads.push_back(
                std::thread(
                    [&]() {
                        Send(serverSocket, username);
                    }
                )
            );
            typing = true;
        }
        if (!isListening)
        {
            Threads.push_back(
                std::thread(
                    [&]() {
                        receive(serverSocket, username);
                    }
                )
            );
            isListening = true;
        }
    }
}


void SocketController::echo(SOCKET& Sendsocket) {
    std::string msg; 
    Threads.push_back(
        std::thread(
            [&]() {
                Send(Sendsocket, "", msg);
            }
        )
    );
    std::cout << "echo" << std::endl;
    //msg = receive(Sendsocket);
    if (msg != "")
    {
        for (auto& sock : linkSockets) {
            std::cout << "message received" << std::endl;
            if (sock != Sendsocket)
            {
                std::cout << "sent to sock" << std::endl;
                Send(sock, "",msg);
            }
        }

    }

}

void SocketController::echoAll(SOCKET& serversocket) {
    while (true)
    {
        for (auto& sock : toCheckSockets) {
            if (sock != serversocket)
            {
                
                echo(sock);
            }
        }
        toCheckSockets.clear();
    }
}


void SocketController::closeAllSockets() {
    for (auto& sock : linkSockets) {
        closesocket(sock);
    }
    linkSockets.clear();
    WSACleanup();
}
