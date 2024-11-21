#include <iostream>

#include "../Include/CPUPlayer.hpp"
#include "../Include/Grid.hpp"

CPUPlayer::CPUPlayer(char symbol) : Player(symbol)
{
	ai = MyAI(symbol);
}

int CPUPlayer::GatherInput(std::function<bool(int)> inputPredicate, Grid& grid)
{
	if (!ai.HasTree())
	{
		ai.SetupTree(grid);
	}

	ai.RemoveOutdatedGrids(grid);

	// int input;
	// do
	//{
	//	std::cout << "Play as " << Symbol << "(CPU) (1 - 9)" << std::endl;
	//	std::cin >> input;

	//} while (inputPredicate(input));*

	//return input - 1;

	return ai.Think(grid);
}
