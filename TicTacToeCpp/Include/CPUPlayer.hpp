#pragma once

#include <functional>

#include "Player.hpp"
#include "MyAI.hpp"
#include "Grid.hpp"

class CPUPlayer : public Player
{
public:
	CPUPlayer() = default;
	CPUPlayer(char symbol);
	int GatherInput(std::function<bool(int)> inputPredicate, Grid& grid, sf::RenderWindow* renderWindow) override;
	void Reset() override;
private:
	MyAI ai;
};
