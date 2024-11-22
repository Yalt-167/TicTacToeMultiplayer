#pragma once

#include <vector>
class Grid
{
public:
	Grid();
	void Place(int slot, bool isO);
	bool IsSlotEmpty(int slot) const;
	bool VerifyWin() const;
	static int EvaluateGrid(const std::vector<std::vector<char>>& grid, char symbolThatShouldWin);
	static int GetDifference(const std::vector<std::vector<char>>& firstGrid, const std::vector<std::vector<char>>& secondGrid);
	static int GetDifference(const Grid& firstGrid, const Grid& secondGrid);
	void Clear();
	void Render() const;
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
};