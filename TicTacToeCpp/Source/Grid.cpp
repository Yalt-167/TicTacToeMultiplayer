#include "Grid.hpp"
#include "Window.hpp"
#include "SpritesData.hpp"

Grid* Grid::instance = nullptr;

Grid::Grid(bool isServerSide_)
{
	instance = this;
	
	if (isServerSide = isServerSide_) { return; }

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
	instance->grid[slot / 3][slot % 3] = isO ? 'O' : 'X';
}

bool Grid::IsSlotEmpty(int slot)
{
	return instance->grid[slot / 3][slot % 3] == ' ';
}

void Grid::RenderToConsole()
{
	for (int row = 0; row < 3; row++)
	{
		std::cout << instance->grid[row][0] << " | " << instance->grid[row][1] << " | " << instance->grid[row][2] << std::endl;

		if (row == 2) { break; }

		std::cout << "--+---+--" << std::endl;
	}

	std::cout << std::endl;
}

void Grid::Render(sf::RenderWindow* renderWindow)
{
	renderWindow->draw(instance->gridSprite);
	sf::Sprite currentSprite;
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			if (instance->grid[row][column] == ' ') { continue; }

			currentSprite = instance->grid[row][column] == 'X' ? instance->crossSprite : instance->circleSprite;
			currentSprite.setPosition(
				(float)(SpritesData::Padding + column * (SpritesData::SymbolSpritesSize + 2 * SpritesData::Padding)),
				(float)(SpritesData::Padding + row * (SpritesData::SymbolSpritesSize + 2 * SpritesData::Padding))
			);
			renderWindow->draw(currentSprite);
		}
	}
}


bool Grid::CheckWin()
{
	for (int i = 0; i < 3; i++)
	{
		if (' ' != instance->grid[i][0] && instance->grid[i][0] == instance->grid[i][1] && instance->grid[i][1] == instance->grid[i][2])
		{
			return true;
		}

		if (' ' != instance->grid[0][i] && instance->grid[0][i] == instance->grid[1][i] && instance->grid[1][i] == instance->grid[2][i])
		{
			return true;
		}
	}

	if (' ' != instance->grid[0][0] && instance->grid[0][0] == instance->grid[1][1] && instance->grid[1][1] == instance->grid[2][2])
	{
		return true;
	}

	if (' ' != instance->grid[0][2] && instance->grid[0][2] == instance->grid[1][1] && instance->grid[1][1] == instance->grid[2][0])
	{
		return true;
	}

	return false;
}

bool Grid::CheckDraw()
{
	for (int i = 0; i < 9; i++)
	{
		if (instance->grid[i / 3][i % 3] == ' ') { return false; }
	}

	return true;
}

void Grid::Clear()
{
	for (int row = 0; row < 3; row++)
	{
		for (int column = 0; column < 3; column++)
		{
			instance->grid[row][column] = ' ';
		}
	}
}
