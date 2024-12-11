#include <iostream>

#include "Grid.hpp"


Grid::Grid()
{
	gridTexture = sf::Texture();
	gridTexture.loadFromFile("Assets/Images/Grid.png");
	gridSprite = sf::Sprite(gridTexture);

	crossTexture = sf::Texture();
	crossTexture.loadFromFile("Assets/Images/Cross.png");
	crossSprite = sf::Sprite(crossTexture);

	circleTexture = sf::Texture();
	circleTexture.loadFromFile("Assets/Images/Circle.png");
	circleSprite = sf::Sprite(circleTexture);
}

void Grid::Place(int slot, bool isO)
{
	grid[slot / 3][slot % 3] = isO ? 'O' : 'X';
}

bool Grid::IsSlotEmpty(int slot) const
{
	return grid[slot / 3][slot % 3] == ' ';
}

void Grid::RenderToConsole() const
{
	for (int row = 0; row < 3; row++)
	{
		std::cout << grid[row][0] << " | " << grid[row][1] << " | " << grid[row][2] << std::endl;

		if (row == 2) { break; }

		std::cout << "--+---+--" << std::endl;
	}

	std::cout << std::endl;
}

void Grid::Render(sf::RenderWindow* renderWindow) const
{

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
	int pointsIfWin = 10;
	int pointsIfLost = -10;
	for (int i = 0; i < 3; i++)
	{
		if (' ' != grid_[i][0] && grid_[i][0] == grid_[i][1] && grid_[i][1] == grid_[i][2])
		{
			return grid_[i][0] == symbolThatShouldWin ? pointsIfWin : pointsIfLost;
		}

		if (' ' != grid_[0][i] && grid_[0][i] == grid_[1][i] && grid_[1][i] == grid_[2][i])
		{
			return grid_[0][i] == symbolThatShouldWin ? pointsIfWin : pointsIfLost;
		}
	}

	if (' ' != grid_[0][0] && grid_[0][0] == grid_[1][1] && grid_[1][1] == grid_[2][2])
	{
		return grid_[0][0] == symbolThatShouldWin ? pointsIfWin : pointsIfLost;
	}

	if (' ' != grid_[0][2] && grid_[0][2] == grid_[1][1] && grid_[1][1] == grid_[2][0])
	{
		return grid_[0][2] == symbolThatShouldWin ? pointsIfWin : pointsIfLost;
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
