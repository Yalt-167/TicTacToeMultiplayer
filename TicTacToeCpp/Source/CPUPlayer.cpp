#include <iostream>

#include "CPUPlayer.hpp"
#include "Grid.hpp"


CPUPlayer::~CPUPlayer()
{
	ai->~MyAI();
	delete ai; // humanity is saved
}

CPUPlayer::CPUPlayer(char symbol) : Player(symbol)
{
	ai = new MyAI(symbol);
}

int CPUPlayer::GatherInput(Grid& grid, sf::RenderWindow*)
{
	std::cout << "Thinking" << std::endl;

	return ai->DoMagic(grid);
}

void CPUPlayer::Reset()
{
	ai->Reset();
}