#pragma once

#include "Grid.hpp"
#include "ServerSocket.hpp"


class GameServer : public Game_
{
public:
	GameServer()
	{

	}

	void Run() override
	{
		serverSocket.Run();

		while (serverSocket.GetConnectedClientsCount() < 2); // await players

		//serverSocket.Broadcast(SerializationHeaders::Start.c_str());

	}
private:
	Grid gameGrid;
	ServerSocket serverSocket;
	int playerTurn = 0;
};