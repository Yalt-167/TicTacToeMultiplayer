#pragma once

class Grid
{
public:
	Grid();
	void Place(int slot, int team);
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