#pragma once

#include "Grid.hpp"
#include "Player.hpp"

class Game
{
public:
	Game() = default;
	Game(bool playerOneIsHuman, bool playerTwoIsHuman, bool doClearConsole_);
	~Game();
	void Run();
	void GatherInput();
	bool InputIsInvalid(int input);
	void VerifyWin();
	void Render() const;
	void Reset();
private:
	Grid grid;
	int turns = 0;
	bool currentPlayerIsO = false;
	bool doClearConsole = true;
	bool playing = true;
	Player** players = new Player*[2];
	std::function<bool(int)> inputValidationPredicate;
};