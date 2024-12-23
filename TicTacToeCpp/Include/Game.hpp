#pragma once

#include "Grid.hpp"
#include "Player.hpp"
#include "Window.hpp"

class Game
{
public:
	Game() = default;
	Game(bool playerOneIsHuman, bool playerTwoIsHuman, bool doClearConsole_);
	~Game();
	void Run();
	void Play();
	bool InputIsInvalid(int input);
	void VerifyWin();
	void Render();
	void Reset();
private:
	Window* window = nullptr;
	Grid grid;
	int turns = 0;
	bool currentPlayerIsO = false;
	bool doClearConsole = true;
	bool playing = true;
	bool twoAIs = false;
	Player** players = new Player*[2];
	//std::function<bool(int)> inputValidationPredicate;
};