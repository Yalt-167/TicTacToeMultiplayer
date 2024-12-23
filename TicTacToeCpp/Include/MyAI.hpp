#pragma once

#include <vector>

#include "Grid.hpp"
#include "Tree.hpp"

class MyAI
{
public:
	MyAI() = default;
	MyAI(char symbol_);
	~MyAI();
	int DoMagic(Grid& currentGrid);
	void Reset();
private:
	char symbol = '@'; // if somehow this show s up im fucked

	// Tree Version
	Node<std::vector<std::vector<char>>>* currentRoot = nullptr;
	bool HasTree() const;
	void SetupTree(Grid& currentGrid);
	void RemoveOutdatedGrids(Grid& currentGrid);
	int ThinkTree(Grid& currentGrid);
	void FillTree(Node<std::vector<std::vector<char>>>* currentNode, bool const isSelfTurn);
	Node<std::vector<std::vector<char>>>* Evaluate();

	// ! Tree Version
	int ThinkNoTree(Grid& currentGrid) const; 
	int Evaluate(bool playing, std::vector<std::vector<char>> rawGrid, int currentInput, int alpha, int beta) const;
};