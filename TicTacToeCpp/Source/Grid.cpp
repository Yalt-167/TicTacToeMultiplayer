#include <iostream>

#include "../Include/Grid.hpp"

//#define TestGrid


Grid::Grid()
{
#ifdef TestGrid
	for (int i = 0; i < 9; i++)
	{
		Place(i, i % 2);
		Render();
	}
#endif
}

void Grid::Place(int slot, int team)
{
	int row = slot % 3;
	grid[row][slot - row * 3] = team > 0 ? 'O' : 'X';
}

bool Grid::IsSlotEmpty(int slot) const
{
	int row = slot % 3;
	return grid[row][slot - row * 3] == ' ';
}


void Grid::Render() const
{
	for (int row = 0; row < 3; row++)
	{
		std::cout << grid[row][0] << " | " << grid[row][1] << " | " << grid[row][2] << std::endl;

		if (row == 2) { break; }

		std::cout << "--+---+--" << std::endl;
	}

	std::cout << "\n\n" << std::endl;
}

bool Grid::VerifyWin() const
{
	for (int i = 0; i < 3; i++)
	{
		if (' ' != grid[i][0] && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
		{
			return true;
		}

		if (' ' != grid[0][i] && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
		{
			return true;
		}
	}

	if (' ' != grid[0][0] && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
	{
		return true;
	}

	if (' ' != grid[0][0] && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][0])
	{
		return true;
	}

	return false;
}

void Grid::Clear()
{
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			grid[row][column] = ' ';
		}
	}
}
