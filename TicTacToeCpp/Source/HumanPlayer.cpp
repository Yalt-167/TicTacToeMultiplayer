#include <iostream>
#include <functional>

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "HumanPlayer.hpp"
#include "SpritesData.hpp"


HumanPlayer::HumanPlayer(char symbol) : Player(symbol) { }


int HumanPlayer::GatherInput(std::function<bool(int)> inputPredicate, Grid&, sf::RenderWindow* renderWindow)
{
	int input = -1;
	/*
	do
	{
		std::cout << "Play as " << Symbol << " (1 - 9)" << std::endl;
		std::cin >> input;

	} while (inputPredicate(input));

	return input - 1;
	*/

	sf::Event event;
	sf::Vector2i mousePos;
	int row;
	int column;
	while (input < 0)
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
					input = 3 * row + column;
			}
		}
	}

	return input;
}
