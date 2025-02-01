#pragma once

#include "Game.hpp"
#include "ServerSocket.hpp"


class GameServer : public Game
{
public:
	GameServer();
	~GameServer();

	void Run() override;

	static void ParsePlay(const int play, int returnBuffer[4], const int clientNumber);
	bool CheckPlay(const int play, const int clientNumber);

	static void RestoreChatMessages(const int clientIndex);
	static void StoreChatMessage(const std::string& msg);

private:
	static GameServer* instance;
	int playerTurn = 0;
	class Grid* gameGrid = nullptr;
	ServerSocket serverSocket;
	std::thread* connectionThread = nullptr;
	int startupPacket[4];
	char gridState[9];

	std::vector<std::string> chatLogs;
};