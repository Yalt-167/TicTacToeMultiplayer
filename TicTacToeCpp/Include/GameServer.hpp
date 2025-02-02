#pragma once

#include "Game.hpp"
#include "ServerSocket.hpp"
#include "GameResult.hpp"


class GameServer : public Game
{
public:
	GameServer();
	~GameServer();

	void Run() override;

	// returns wether game ended
	static void ParsePlay(const int play, int returnBuffer[3], const int clientNumber);
	static void EndGame(const GameResult gameResult, const int clientNumber);
	bool CheckPlay(const int play, const int clientNumber);
	static void RestoreChatMessages(const int clientIndex);
	static void StoreChatMessage(const std::string& msg);

private:
	static GameServer* instance;
	int playerTurn = 0;
	class Grid* gameGrid = nullptr;
	ServerSocket serverSocket;
	std::thread* connectionThread = nullptr;
	int startupPacket[3];
	int score[2]{ 0, 0 };
	char gridState[9];
	std::vector<std::string> chatLogs;
};