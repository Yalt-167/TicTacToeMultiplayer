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

void Grid::Place(int slot, bool isO)
{
	grid[slot / 3][slot % 3] = isO ? 'O' : 'X';
}

bool Grid::IsSlotEmpty(int slot) const
{
	return grid[slot / 3][slot % 3] == ' ';
}

void Grid::Render() const
{
	for (int row = 0; row < 3; row++)
	{
		std::cout << grid[row][0] << " | " << grid[row][1] << " | " << grid[row][2] << std::endl;

		if (row == 2) { break; }

		std::cout << "--+---+--" << std::endl;
	}

	std::cout << std::endl;
}

std::vector<std::vector<char>> Grid::GetRaw() const
{
	return grid;
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

	if (' ' != grid[0][2] && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
	{
		return true;
	}

	return false;
}

// STATIC but this dogshit language doesn t let me mark it properly from here
int Grid::EvaluateGrid(const std::vector<std::vector<char>>& grid_, char symbolThatShouldWin)
{
	for (int i = 0; i < 3; i++)
	{
		if (' ' != grid_[i][0] && grid_[i][0] == grid_[i][1] && grid_[i][1] == grid_[i][2])
		{
			return grid_[i][0] == symbolThatShouldWin ? 10 : -10;
		}

		if (' ' != grid_[0][i] && grid_[0][i] == grid_[1][i] && grid_[1][i] == grid_[2][i])
		{
			return grid_[0][i] == symbolThatShouldWin ? 10 : -10;
		}
	}

	if (' ' != grid_[0][0] && grid_[0][0] == grid_[1][1] && grid_[1][1] == grid_[2][2])
	{
		return grid_[0][0] == symbolThatShouldWin ? 10 : -10;
	}

	if (' ' != grid_[0][2] && grid_[0][2] == grid_[1][1] && grid_[1][1] == grid_[2][0])
	{
		return grid_[0][2] == symbolThatShouldWin ? 10 : -10;
	}

	return 0;
}

// STATIC
int Grid::GetDifference(const std::vector<std::vector<char>>& firstGrid, const std::vector<std::vector<char>>& secondGrid)
{
	for (int i = 0; i < 9; i++)
	{
		if (firstGrid[i / 3][i % 3] != secondGrid[i / 3][i % 3])
		{
			return i;
		}
	}

	throw new std::runtime_error("Same grid somehow");
}
// STATIC
int Grid::GetDifference(const Grid& firstGrid, const Grid& secondGrid)
{
	for (int i = 0; i < 9; i++)
	{
		if (firstGrid.grid[i / 3][i % 3] != secondGrid.grid[i / 3][i % 3])
		{
			return i;
		}
	}

	throw new std::runtime_error("Same grid somehow");
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
