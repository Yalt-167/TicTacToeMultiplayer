#pragma once


struct SpritesData
{
public:
	static void Init(const int gridSpriteSize, const int symboldSpritesSize);
	static int GridSpriteSize;
	static int CellSize;
	static int SymbolSpritesSize;
	static int Padding;
};