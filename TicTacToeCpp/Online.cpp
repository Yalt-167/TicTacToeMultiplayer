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


SOCKET SocketController::createSocket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    sockets.push_back(sock);
    return sock;
}


bool SocketController::Bind(SOCKET sock, int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accepte toutes les connexions entrantes.
    serverAddr.sin_port = htons(port); // Port à écouter.

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


bool SocketController::Listen(SOCKET sock) {
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
    
    speakThreads.push_back(std::thread(&SocketController::speak, this, clientSocket));
    return true;
}


bool SocketController::Send(SOCKET sock, std::string message, std::string username) {
    int bytesSent = send(sock, (username + " : " + message).c_str(), strlen((username + " : " + message).c_str()), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << "Message sent" << std::endl;
    return true;
}


char* SocketController::receive(SOCKET sock)
{
    int bufferSize = 512;
    char* buffer = new char[bufferSize];
    while (true)
    {
        int bytesReceived = recv(sock, buffer, bufferSize - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Null-terminate le message reçu.
            std::cout << buffer << std::endl;
            break;
            return buffer;
        }
        else if (bytesReceived == 0) {
            std::cout << "Connection closed by peer." << std::endl;
        }

    }
    /*
    else {
        std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
    }*/
}

void SocketController::speak(SOCKET clientSocket) {
    while (true)
    {
        char* msg = receive(clientSocket);
        for (const SOCKET& sock : sockets) {
            if (sock != clientSocket) {
                send(sock, msg, strlen(msg), 0);
                std::cout << "test" << std::endl;
            }
        }
    }
}




void SocketController::closeAll(SOCKET clientSocket, SOCKET serverSocket) {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}