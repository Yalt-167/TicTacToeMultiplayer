#include "Online.hpp"

//initialise WinSock2
bool initWinSock2() {

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }

    return true;
}

//creer une socket
SOCKET createSocket() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    return sock;
}

//prépare un port a une connection(serveur)
bool bind(SOCKET sock, int port) {
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

//connect une socket a un port(client)
bool connect(SOCKET sock, int port) {
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

//met en écoute la socket(serveur)
bool listen(SOCKET sock) {
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }
    return true;
}

//rempli la socket(client)
bool accept(SOCKET& clientSocket, SOCKET serverSocket) {
    clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    return true;
}
//envoie un message sur la socket serveur(client)
bool send(SOCKET sock, std::string message) {
    int bytesSent = send(sock, message.c_str(), strlen(message.c_str()), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

//recoi un message sur une socket d'un client(serveur)
void receive(SOCKET sock) {
    char buffer[512];
    int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate le message reçu.
        std::cout << "Received: " << buffer << std::endl;
    }
    else if (bytesReceived == 0) {
        std::cout << "Connection closed by peer." << std::endl;
    }
    else {
        std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
    }
}



void closeAll(SOCKET clientSocket, SOCKET serverSocket) {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}