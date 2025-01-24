#include <iostream>

#include "Grid.hpp"
#include "Window.hpp"
#include "SpritesData.hpp"


Grid::Grid()
{
	gridTexture = sf::Texture();
	gridTexture.loadFromFile("Assets/Images/Grid.png");
	gridSprite = sf::Sprite(gridTexture);
	gridSprite.setPosition(0, 0);

	crossTexture = sf::Texture();
	crossTexture.loadFromFile("Assets/Images/Cross.png");
	crossSprite = sf::Sprite(crossTexture);

	circleTexture = sf::Texture();
	circleTexture.loadFromFile("Assets/Images/Circle.png");
	circleSprite = sf::Sprite(circleTexture);

	SpritesData::Init(gridTexture.getSize().x, crossTexture.getSize().x);
}

void Grid::Place(int slot, bool isO)
{
	grid[slot / 3][slot % 3] = isO ? 'O' : 'X';
}

bool Grid::IsSlotEmpty(int slot) const
{
	return grid[slot / 3][slot % 3] == ' ';
}

//STATIC
bool Grid::IsSlotEmpty(const std::vector<std::vector<char>>& grid, int slot)
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

void Grid::Render(sf::RenderWindow* renderWindow)
{
	renderWindow->draw(gridSprite);
	sf::Sprite currentSprite;
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			if (grid[row][column] == ' ') { continue; }

			currentSprite = grid[row][column] == 'X' ? crossSprite : circleSprite;
			currentSprite.setPosition(
				(float)(SpritesData::Padding + column * (SpritesData::SymbolSpritesSize + 2 * SpritesData::Padding)),
				(float)(SpritesData::Padding + row * (SpritesData::SymbolSpritesSize + 2 * SpritesData::Padding))
			);
			renderWindow->draw(currentSprite);
		}
	}
}

std::vector<std::vector<char>> Grid::GetRaw() const
{
	return grid;
}

bool Grid::VerifyWin() const
{
	return Grid::CheckRawGrid(GetRaw());
}

bool Grid::CheckRawGrid(const std::vector<std::vector<char>>& grid)
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

bool Grid::CheckWetherGridFull(const std::vector<std::vector<char>>& grid)
{
	for (int i = 0; i < 9; i++)
	{
		if (grid[i / 3][i % 3] == ' ') { return false; }
	}

	return true;
}

// STATIC but this dogshit language doesn t let me mark it properly from here
int Grid::EvaluateGrid(const std::vector<std::vector<char>>& grid_, char symbolThatShouldWin)
{
	for (int i = 0; i < 3; i++)
	{
		if (' ' != grid_[i][0] && grid_[i][0] == grid_[i][1] && grid_[i][1] == grid_[i][2])
		{
			return grid_[i][0] == symbolThatShouldWin ? 1 : -1;
		}

		if (' ' != grid_[0][i] && grid_[0][i] == grid_[1][i] && grid_[1][i] == grid_[2][i])
		{
			return grid_[0][i] == symbolThatShouldWin ? 1 : -1;
		}
	}

	if (' ' != grid_[0][0] && grid_[0][0] == grid_[1][1] && grid_[1][1] == grid_[2][2])
	{
		return grid_[0][0] == symbolThatShouldWin ? 1 : -1;
	}

	if (' ' != grid_[0][2] && grid_[0][2] == grid_[1][1] && grid_[1][1] == grid_[2][0])
	{
		return grid_[0][2] == symbolThatShouldWin ? 1 : -1;
	}

	return 0;
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
