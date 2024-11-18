#include <iostream>

#include "../Include/Game.hpp"

void Game::Run()
{
	while (playing)
	{
		GatherInput();
		Render();
		VerifyWin();
		turns++;
		currentPlayer = !currentPlayer;
	}
}

void Game::GatherInput()
{
	int input;
	do
	{
		std::cout << "Play as " << (currentPlayer > 0 ? 'O' : 'X') << " (1 - 9)" << std::endl;
		std::cin >> input;
	} while (input < 1 || input > 9 || !grid.IsSlotEmpty(input - 1));

	grid.Place(input - 1, currentPlayer);
}

void Game::VerifyWin()
{
	if (grid.VerifyWin())
	{
		std::cout << (currentPlayer > 0 ? 'O' : 'X') << "Has won" << std::endl;
		turns = -1;
		grid.Clear();
	}

	if (turns == 8)
	{
		std::cout << "Its a Draw" << std::endl;
		turns = -1;
		grid.Clear();
	}
		
}

void Game::Render()
{
	grid.Render();
}
