#pragma once

#include <functional>

class Player
{
public:
	Player() = default;
	Player(char symbol);
	virtual int GatherInput(std::function<bool(int)>inputPredicate) = 0;
	char Symbol;
};