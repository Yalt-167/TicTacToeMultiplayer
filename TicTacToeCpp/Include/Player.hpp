#pragma once

#include "Grid.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player
{
public:
	Player() = default;
	Player(char symbol);
	virtual int GatherInput(Grid& grid, sf::RenderWindow* renderWindow) = 0;
	virtual void Reset();
	char Symbol;
};