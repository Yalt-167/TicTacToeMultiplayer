#pragma once

#include <functional>

#include "Player.hpp"

class HumanPlayer : public Player
{
public:
	HumanPlayer() = default;
	HumanPlayer(char symbol);
	int GatherInput(std::function<bool(int)> inputPredicate, Grid&) override;
};