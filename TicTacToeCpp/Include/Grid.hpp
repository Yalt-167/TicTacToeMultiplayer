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
	std::vector<std::vector<char>> GetRaw() const;
private:
	std::vector<std::vector<char>> grid = {
		{' ', ' ', ' ' },
		{' ', ' ', ' ' },
		{' ', ' ', ' ' },
	};
};