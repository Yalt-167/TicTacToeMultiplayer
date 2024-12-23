#include "SpritesData.hpp"

int SpritesData::GridSpriteSize = 0;
int SpritesData::CellSize = 0;
int SpritesData::SymbolSpritesSize = 0;
int SpritesData::Padding = 0;

void SpritesData::Init(int gridSpriteSize, int symboldSpritesSize)
{
	GridSpriteSize = gridSpriteSize; // 600
	CellSize = GridSpriteSize / 3;
	SymbolSpritesSize = symboldSpritesSize; // 170
	Padding = (CellSize - SymbolSpritesSize) / 2;
}