#pragma once

#include <functional>

class Grid;

class Player
{
public:
	Player() = default;
	Player(char symbol);
	virtual int GatherInput(std::function<bool(int)>inputPredicate, Grid& grid) = 0;
	virtual void Reset();
	char Symbol;
};