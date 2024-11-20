#pragma once

#include <functional>
#include "Player.hpp"

class CPUPlayer : public Player
{
public:
	CPUPlayer() = default;
	CPUPlayer(char symbol);
	int GatherInput(std::function<bool(int)> inputPredicate) override;
};
