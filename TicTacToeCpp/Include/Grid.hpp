#pragma once

class Grid
{
public:
	Grid();
	void Place(int slot, bool isO);
	bool IsSlotEmpty(int slot) const;
	bool VerifyWin() const;
	void Clear();
	void Render() const;
private:
	char grid[3][3] = {
		{' ', ' ', ' ' },
		{' ', ' ', ' ' },
		{' ', ' ', ' ' },
	};
};