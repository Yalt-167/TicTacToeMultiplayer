#pragma once

#include "Grid.hpp"
#include "ServerSocket.hpp"


class GameServer : public Game_
{
public:
	GameServer() = default;

	void Run() override;

private:
	Grid gameGrid;
	ServerSocket serverSocket;
	int playerTurn = 0;
};