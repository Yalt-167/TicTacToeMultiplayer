#include <iostream>

#include "CPUPlayer.hpp"
#include "Grid.hpp"

CPUPlayer::CPUPlayer(char symbol) : Player(symbol)
{
	ai = MyAI(symbol);
}

int CPUPlayer::GatherInput(std::function<bool(int)> inputPredicate, Grid& grid, sf::RenderWindow*)
{
	std::cout << "Thinking" << std::endl;

	if (!ai.HasTree())
	{
		ai.SetupTree(grid);
	}
	else
	{
		ai.RemoveOutdatedGrids(grid);
	}

	return ai.Think(grid);
}

void CPUPlayer::Reset()
{
	ai.Reset();
}
