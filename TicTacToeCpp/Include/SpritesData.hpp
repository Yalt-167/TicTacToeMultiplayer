#pragma once

struct SpritesData
{
public:
	static void Init(int gridSpriteSize, int symboldSpritesSize);
	static int GridSpriteSize;
	static int CellSize;
	static int SymbolSpritesSize;
	static int Padding;
};