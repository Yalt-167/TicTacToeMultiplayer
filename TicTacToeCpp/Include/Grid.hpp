#pragma once


class Grid
{
public:
	Grid(const bool isServerSide_);
	static void Place(const int slot, const  bool isO);
	static bool IsSlotEmpty(const int slot);
	static bool CheckWin();
	static bool CheckDraw();
	static void Clear();
	static void RenderToConsole();
	static void Render(sf::RenderWindow* renderWindow);
	static char* Serialize(char receiveGridBuffer[9]);
	static void Deserialize(char grid[9]);
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