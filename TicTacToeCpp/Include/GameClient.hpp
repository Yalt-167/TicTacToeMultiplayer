#pragma once

#include "Game.hpp"

class GameClient : public Game
{
public:
	GameClient(const std::string& userName_);
	~GameClient();

	static void HandlePlayResult(const int gameResult, const int play, const bool canPlay, const int symbol);
	
private:
	void Run() override;

	void Play();
	void Render();

	int GatherInput() const;

	static GameClient* instance;
	class ClientSocket* clientSocket;
	std::string userName;
	bool canPlay = false;
	bool shouldRun = true;

	class Window* window = nullptr;
	class Grid* grid;
	const int INVALID_PLAY = -1;
	const int QUIT = -2;
};