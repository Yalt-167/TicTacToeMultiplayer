#pragma once

#include "Grid.hpp"
#include "Player.hpp"
#include "Window.hpp"

class Game
{
public:
	Game(bool isServer_);
	~Game();
	void Run();
	void Play();
	void VerifyWin();
	void Render();
	void Reset();
private:
	bool isServer;
	Window* window = nullptr;
	Grid grid;
	int turns = 0;
	bool currentPlayerIsO = false;
	bool doClearConsole = true;
	bool playing = true;
	Player players[2]
	{
		Player('X'),
		Player('O')
	};
	//std::function<bool(int)> inputValidationPredicate;
};