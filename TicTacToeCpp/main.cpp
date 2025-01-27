#include <iostream>
#include <string>


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


	SOCKET serverSocket = socketcontroller.createSocket();
	SOCKET clientSocket = socketcontroller.createSocket();

	std::string username;
	if (isServer)
	{

		socketcontroller.Bind(serverSocket, 8080);
		socketcontroller.Listen(serverSocket);
		std::cout << "server set" << std::endl;

		while (true)
		{
			socketcontroller.Accept(clientSocket, serverSocket);
			
		}
	}
	else if (!isServer)
	{

		std::cout << "Enter your username: " << std::endl;
		std::cin >> username;

		if (!socketcontroller.Connect(clientSocket, 8080)) { std::cerr << "Failed to connect" << std::endl; }
		while (true){
			std::string msg;
			std::cout << username << " : ";
			std::getline(std::cin, msg);
			socketcontroller.Send(clientSocket, msg, username);
			std::thread t(&SocketController::receive, &socketcontroller, serverSocket);

			t.join();
		}
	}

    //Game(isServer).Run();
	
	return 0;
}
