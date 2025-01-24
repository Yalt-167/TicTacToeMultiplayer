#pragma once

#include <iostream>
#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/window.hpp"
#include "SFML/system.hpp"

class Grid
{
public:
	Grid();
	void Place(int slot, bool isO);
	bool IsSlotEmpty(int slot) const;
	static bool IsSlotEmpty(const std::vector<std::vector<char>>& grid, int slot);
	bool VerifyWin() const;
	static bool CheckRawGrid(const std::vector<std::vector<char>>& grid);
	static bool CheckWetherGridFull(const std::vector<std::vector<char>>& grid);
	static int EvaluateGrid(const std::vector<std::vector<char>>& grid, char symbolThatShouldWin);
	void Clear();
	void RenderToConsole() const;
	void Render(sf::RenderWindow* renderWindow);
	static void RenderStatic(const std::vector<std::vector<char>>& grid_)
	{
		for (int row = 0; row < 3; row++)
		{
			std::cout << grid_[row][0] << " | " << grid_[row][1] << " | " << grid_[row][2] << std::endl;

			if (row == 2) { break; }

			std::cout << "--+---+--" << std::endl;
		}

		std::cout << std::endl;
	}
	std::vector<std::vector<char>> GetRaw() const;
private:
	std::vector<std::vector<char>> grid = {
		{' ', ' ', ' ' },
		{' ', ' ', ' ' },
		{' ', ' ', ' ' },
	};

	sf::Sprite gridSprite;
	sf::Texture gridTexture;
	sf::Sprite crossSprite;
	sf::Texture crossTexture;
	sf::Sprite circleSprite;
	sf::Texture circleTexture;
};