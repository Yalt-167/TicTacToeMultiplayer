#include "Player.hpp"
#include "SpritesData.hpp"


Player::Player(char symbol)
{
	Symbol = symbol;
}

int Player::GatherInput(Grid& grid, sf::RenderWindow* renderWindow)
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
