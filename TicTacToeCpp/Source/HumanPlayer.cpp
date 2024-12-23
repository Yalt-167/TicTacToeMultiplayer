#include <iostream>
#include <functional>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "HumanPlayer.hpp"
#include "SpritesData.hpp"


HumanPlayer::HumanPlayer(char symbol) : Player(symbol) { }


int HumanPlayer::GatherInput(Grid& grid, sf::RenderWindow* renderWindow)
{
	int validatedInput = -1;
	int rawInput = -1;
	
	sf::Event event;
	sf::Vector2i mousePos;
	int row;
	int column;
	while (validatedInput < 0)
	{
		while (renderWindow->pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					renderWindow->close();
					return 0;

				case sf::Event::MouseButtonPressed:
					if (event.key.code != sf::Mouse::Button::Left) { break; }

					mousePos = sf::Mouse::getPosition(*renderWindow);

					column = mousePos.x / SpritesData::CellSize;
					row = mousePos.y / SpritesData::CellSize;

					rawInput = 3 * row + column;

					if (grid.IsSlotEmpty(rawInput))
					{
						validatedInput = rawInput;
					}
			}
		}
	}

	return validatedInput;
}
