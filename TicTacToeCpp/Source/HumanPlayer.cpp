#include <iostream>
#include <functional>

#include "../Include/HumanPlayer.hpp"

HumanPlayer::HumanPlayer(char symbol) : Player(symbol) { }


int HumanPlayer::GatherInput(std::function<bool(int)> inputPredicate)
{
	int input;
	do
	{
		std::cout << "Play as " << Symbol << " (1 - 9)" << std::endl;
		std::cin >> input;

	} while (inputPredicate(input));

	return input - 1;
}
