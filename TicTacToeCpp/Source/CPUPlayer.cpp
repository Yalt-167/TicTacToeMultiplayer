#include <iostream>

#include "../Include/CPUPlayer.hpp"

CPUPlayer::CPUPlayer(char symbol) : Player(symbol) { }

int CPUPlayer::GatherInput(std::function<bool(int)> inputPredicate)
{
	int input;
	do
	{
		std::cout << "Play as " << Symbol << "(CPU) (1 - 9)" << std::endl;
		std::cin >> input;

	} while (inputPredicate(input));

	return input - 1;
}
