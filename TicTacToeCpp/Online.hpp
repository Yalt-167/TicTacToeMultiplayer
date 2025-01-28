#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>





class SocketController
{
	std::vector<std::thread> speakThreads;
	std::vector<SOCKET> sockets;


public:
	//initialise WinSock2
	bool initWinSock2();

	//creer une socket
	SOCKET createSocket();

	//prépare un port a une connection(serveur)
	bool Bind(SOCKET sock, int port);

	//connect une socket a un port(client)
	bool Connect(SOCKET& sock, int port);

	//met en écoute la socket(serveur)
	bool Listen(SOCKET sock);

	//rempli la socket(client)
	bool Accept(SOCKET& clientSocket, SOCKET& serverSocket);

	//envoie un message sur la socket serveur(client)
	bool Send(SOCKET sock, std::string message, std::string username);

	//recoi un message sur une socket d'un client(serveur)
	char* receive(SOCKET sock);

	//interaction avec le serveur
	void speak(SOCKET clientSocket);

	//ferme les sockets et WinSock2
	void closeAllSockets();

};




