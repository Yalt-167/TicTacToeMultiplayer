#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>


bool initWinSock2();

SOCKET createSocket();

bool bind(SOCKET sock, int port);

bool connect(SOCKET sock, int port);

bool listen(SOCKET sock);

bool accept(SOCKET& clientSocket, SOCKET serverSocket);

bool send(SOCKET sock, std::string message);

void receive(SOCKET sock);

void closeAll(SOCKET clientSocket, SOCKET serverSocket);