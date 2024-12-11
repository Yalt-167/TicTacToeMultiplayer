#include <iostream>

#include "SpritesData.hpp"

int SpritesData::GridSpriteSize = 0;
int SpritesData::CellSize = 0;
int SpritesData::SymbolSpritesSize = 0;
int SpritesData::Padding = 0;

void SpritesData::Init(int gridSpriteSize, int symboldSpritesSize)
{
	GridSpriteSize = 600;
	GridSpriteSize = gridSpriteSize;
	std::cout << gridSpriteSize << std::endl;
	CellSize = GridSpriteSize / 3;
	SymbolSpritesSize = 170;
	SymbolSpritesSize = symboldSpritesSize;
	std::cout << SymbolSpritesSize << std::endl;
	Padding = CellSize - SymbolSpritesSize;
}