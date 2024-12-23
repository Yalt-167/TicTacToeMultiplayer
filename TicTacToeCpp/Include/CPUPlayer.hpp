#pragma once

#include "Player.hpp"
#include "MyAI.hpp"
#include "Grid.hpp"

class CPUPlayer : public Player
{
public:
	CPUPlayer() = default;
	~CPUPlayer();
	CPUPlayer(char symbol);
	int GatherInput(Grid& grid, sf::RenderWindow* renderWindow) override;
	void Reset() override;
private:
	MyAI* ai = nullptr;
};
