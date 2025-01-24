#pragma once

#include "Grid.hpp"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Player
{
public:
	Player(char symbol);
	int GatherInput(Grid& grid, sf::RenderWindow* renderWindow);
	char Symbol;
};