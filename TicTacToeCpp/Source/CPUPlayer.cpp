#include <iostream>

#include "../Include/CPUPlayer.hpp"

CPUPlayer::CPUPlayer(char symbol) : Player(symbol) { }

int CPUPlayer::GatherInput(std::function<bool(int)> inputPredicate)
{
	std::cout << "Hi from CPU" << std::endl;

	int input;
	do
	{
		std::cout << "Play as " << Symbol << " (1 - 9)" << std::endl;
		std::cin >> input;

	} while (inputPredicate(input));

	return input - 1;
}
