#pragma once

#include <vector>
#include <functional>

#include "Grid.hpp"
#include "Tree.hpp"

class MyAI
{
public:
	MyAI() = default;
	MyAI(char symbol_);
	~MyAI();
	bool HasTree() const;
	void SetupTree(Grid& currentGrid);
	void RemoveOutdatedGrids(Grid& currentGrid);
	int Think(Grid& currentGrid);
	void Reset();
	Node<std::vector<std::vector<char>>>* currentRoot = nullptr;
private:
	void FillTree(Node<std::vector<std::vector<char>>>* currentNode, bool const isSelfTurn);
	void RenderGridLikeARetard(Node<std::vector<std::vector<char>>>* node)
	{
		for (int row = 0; row < 3; row++)
		{
			std::cout << node->Value[row][0] << " | " << node->Value[row][1] << " | " << node->Value[row][2] << std::endl;

			if (row == 2) { break; }

			std::cout << "--+---+--" << std::endl;
		}

		std::cout << std::endl;
	}
	Node<std::vector<std::vector<char>>>* Evaluate();
	char symbol = '@'; // if somehow this shows up im fucked
};