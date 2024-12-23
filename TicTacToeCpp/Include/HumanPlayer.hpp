#pragma once

#include <functional>

#include "Player.hpp"

class HumanPlayer : public Player
{
public:
	HumanPlayer() = default;
	HumanPlayer(char symbol);
	int GatherInput(Grid& grid, sf::RenderWindow* renderWindow) override;
};