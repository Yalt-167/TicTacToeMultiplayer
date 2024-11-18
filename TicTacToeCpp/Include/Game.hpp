#pragma once

#include "Grid.hpp"

class Game
{
public:
	Game() = default;
	void Run();
	void GatherInput();
	void VerifyWin();
	void Render();
private:
	Grid grid;
	int turns = 0;
	bool currentPlayer = false;
	bool playing = true;
};