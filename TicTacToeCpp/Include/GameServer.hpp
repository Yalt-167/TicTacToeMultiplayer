#pragma once

#include "Game.hpp"
#include "Grid.hpp"
#include "ServerSocket.hpp"
#include "PacketSendTarget.hpp"


class GameServer : public Game
{
public:
	GameServer();
	~GameServer();

	void Run() override;

	static void ParsePlay(int play, int returnBuffer[4], int clientNumber);
	bool CheckPlay(int play, int clientNumber);

private:
	static GameServer* instance;
	int playerTurn = 0;
	Grid* gameGrid;
	ServerSocket serverSocket;
	std::thread* connectionThread = nullptr;
};