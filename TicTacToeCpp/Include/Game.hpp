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
	void Reset();
private:
	Grid grid;
	int turns = 0;
	bool currentPlayerIsO = false;
	bool playing = true;
};