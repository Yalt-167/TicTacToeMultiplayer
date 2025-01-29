#pragma once


class Grid
{
public:
	Grid(bool isServerSide_);
	static void Place(int slot, bool isO);
	static bool IsSlotEmpty(int slot);
	static bool CheckWin();
	static bool CheckDraw();
	static void Clear();
	static void RenderToConsole();
	static void Render(sf::RenderWindow* renderWindow);
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

	static Grid* instance;
	bool isServerSide = false;
};