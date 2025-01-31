#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <string>





class SocketController
{
	std::vector<std::thread> Threads;
	std::vector<SOCKET> linkSockets;
	std::vector<SOCKET> toCheckSockets;
	bool typing = false;
	bool isListening = false;
	sockaddr_in serverAddr;
public:
	//initialise WinSock2
	bool initWinSock2();

	//creer une socket
	SOCKET createSocket(int port);

	//prépare un port a une connection(serveur)
	bool Bind(SOCKET& sock);

	//connect une socket a un port(client)
	bool Connect(SOCKET& sock, int port);

	//met en écoute la socket(serveur)
	bool Listen(SOCKET& sock);

	//rempli la socket(client)
	bool Accept(SOCKET& clientSocket, SOCKET& serverSocket);

	//envoie un message sur la socket serveur(client)
	bool Send(SOCKET& sock, std::string username = "",std:: string message = "");

	//recoi un message sur une socket d'un client(serveur)
	char* receive(SOCKET& sock, std::string message = "");

	//interaction avec le serveur
	void speak(SOCKET& serverSocket, std::string username);

	//renvoie le message a une autre socket
	void echo(SOCKET& sock);

	//renvoie le message aux autres socket
	void echoAll(SOCKET& serversocket);

	//ferme les sockets et WinSock2
	void closeAllSockets();

};




