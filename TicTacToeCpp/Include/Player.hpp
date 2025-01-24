#pragma once

#include "Grid.hpp"

class Player
{
public:
	Player(char symbol);
	int GatherInput(Grid& grid, sf::RenderWindow* renderWindow);
	char Symbol;
};