#include <iostream>


#include "SFML/Graphics.hpp"
#include "SFML/window.hpp"
#include "SFML/system.hpp"

#include "Online.hpp"
#include "Grid.hpp"
#include "Game.hpp"


int main(int argc, char** argv)
{
	if (argc < 2) { throw std::exception("Don t ever waste my time like this"); }
	SocketController socketcontroller;

	socketcontroller.initWinSock2();

	//bool isServer = strcmp(argv[1], "Server") == 0;

	std::string typeServer;
	std::cin >> typeServer;
	bool isServer = strcmp(typeServer.c_str(), "server") == 0;


	SOCKET serverSocket = socketcontroller.createSocket(8080);	

	std::string username;
	if (isServer)
	{
		socketcontroller.Bind(serverSocket);
		socketcontroller.Listen(serverSocket);
		std::cout << "server set" << std::endl;
		socketcontroller.echoAll(serverSocket);

	}
	else if (!isServer)
	{
		SOCKET clientSocket = socketcontroller.createSocket(8080);
		std::cout << "Enter your username: " << std::endl;
		std::cin >> username;
		if (!socketcontroller.Connect(clientSocket, 8080)) { std::cerr << "Failed to connect" << std::endl; }
		socketcontroller.speak(serverSocket, username);
	}

    //Game(isServer).Run();
	
	return 0;
}
