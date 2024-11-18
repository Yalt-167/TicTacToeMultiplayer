#include <iostream>
#include <cstdlib>

#include "../Include/Game.hpp"

void Game::Run()
{
	while (playing)
	{
		GatherInput();
		Render();
		VerifyWin();
	}
}

void Game::GatherInput()
{
	int input;
	do
	{
		std::cout << "Play as " << (currentPlayerIsO ? 'O' : 'X') << " (1 - 9)" << std::endl;
		std::cin >> input;
		
	} while (InputIsInvalid(input));

	grid.Place(input - 1, currentPlayerIsO);
	turns++;
}

bool Game::InputIsInvalid(int input)
{
	if (input < 1)
	{
		std::cout << "This input is too small" << std::endl;
		return true;
	}

	if (input > 9)
	{
		std::cout << "This input is too big" << std::endl;
		return true;
	}

	if (!grid.IsSlotEmpty(input - 1))
	{
		std::cout << "There is a symbol in this slot already" << std::endl;
		return true;
	}

	return false;
}

void Game::VerifyWin()
{
	if (grid.VerifyWin())
	{
		std::cout << (currentPlayerIsO > 0 ? 'O' : 'X') << " has won" << std::endl;
		Reset();
	}

	if (turns == 9)
	{
		std::cout << "Its a Draw" << std::endl;
		Reset();
	}
		
	currentPlayerIsO = !currentPlayerIsO;
}

void Game::Render()
{
	system("cls");
	grid.Render();
}

void Game::Reset()
{
	turns = 0;
	grid.Clear();
}
