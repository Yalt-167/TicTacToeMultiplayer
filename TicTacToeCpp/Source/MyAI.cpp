#include <iostream>
#include <limits>

#include "MyAI.hpp"
#include "Tree.hpp"

#define WITH_TREE
// not meant to be in the final assignment but I had fun so it s staying


MyAI::MyAI(char symbol_)
{
	symbol = symbol_;
}

MyAI::~MyAI()
{
	Reset();
}

int MyAI::DoMagic(Grid& currentGrid)
{
#ifdef WITH_TREE
	

	return ThinkTree(currentGrid);
#else
	return ThinkNoTree(currentGrid);
#endif // WITH_TREE
}

bool MyAI::HasTree() const
{
	return currentRoot != nullptr;
}

void MyAI::SetupTree(Grid& currentGrid)
{
	currentRoot = new Node<std::vector<std::vector<char>>>(currentGrid.GetRaw());
	FillTree(currentRoot, true);
}

void MyAI::FillTree(Node<std::vector<std::vector<char>>>* node, bool const isSelfTurn)
{
	for (int i = 0; i < 9; i++)
	{
		if (node->Value[i / 3][i % 3] == ' ')
		{
			Node<std::vector<std::vector<char>>>* child = new Node<std::vector<std::vector<char>>>(node->Value);
			child->Value[i / 3][i % 3] = isSelfTurn ? symbol : symbol == 'X' ? 'O' : 'X';
			node->AddChild(child);

			FillTree(child, !isSelfTurn);
		}
	}
}

Node<std::vector<std::vector<char>>>* MyAI::Evaluate()
{
	int bestScore = INT_MIN;
	Node<std::vector<std::vector<char>>>* bestNode = nullptr;

	for (Node<std::vector<std::vector<char>>>* node : currentRoot->Children)
	{
		int currentScore = node->Evaluate(Grid::EvaluateGrid, symbol);
		
		if (currentScore > bestScore)
		{
			bestNode = node;
			bestScore = currentScore;
		}
	}

	return bestNode;
}

void MyAI::RemoveOutdatedGrids(Grid& currentGrid)
{
	std::vector<std::vector<char>> currentRawGrid = currentGrid.GetRaw();
	
	Node<std::vector<std::vector<char>>>* newRoot = nullptr;
	for (int i = (int)currentRoot->Children.size() - 1; i > -1; i--) // do it backward to avoid error with removing elements while iterating over it
	{
		if (currentRoot->Children[i]->Value == currentRawGrid)
		{
			newRoot = currentRoot->Children[i];
		}
		else
		{
			currentRoot->RemoveChild(currentRoot->Children[i]);
		}
	}

	currentRoot = newRoot;
}

int MyAI::ThinkTree(Grid& currentGrid)
{
	if (!HasTree())
	{
		SetupTree(currentGrid);
	}
	else
	{
		RemoveOutdatedGrids(currentGrid);
	}

	currentRoot = Evaluate();
	currentRoot->RemoveParent(true);

	return Grid::GetDifference(currentGrid.GetRaw(), currentRoot->Value);
}

void MyAI::Reset()
{
	if (currentRoot == nullptr) { return; } // used the NoTree version so nothing to cleanup

	currentRoot->RemoveChildren();
	
	delete currentRoot;
	currentRoot = nullptr;
}


int MyAI::ThinkNoTree(Grid& grid) const
{
	int alpha = -1000;
	int bestInput;
	for (int i = 0; i < 9; i++)
	{
		if (!grid.IsSlotEmpty(i)) { continue; }

		int value = Evaluate(true, grid.GetRaw(), i, alpha, 1000);

		if (value == 1) { return i; } // means that the AI wins

		if (alpha < value) // otherwise check wether this input was better than the previous
		{
			bestInput = i;
			alpha = value;
		}
	}

	return bestInput;
}

int MyAI::Evaluate(bool isPlaying, std::vector<std::vector<char>> rawGrid, int currentInput, int alpha, int beta) const
{
	rawGrid[currentInput / 3][currentInput % 3] = isPlaying ? symbol : symbol == 'X' ? 'O' : 'X';

	int winning = Grid::EvaluateGrid(rawGrid, symbol);
	if (winning != 0) // if one or the other player won thanks to <currentInput> 
	{
		return winning;
	}

	if (Grid::CheckWetherGridFull(rawGrid)) { return 0; }

	int value = isPlaying ? -1000 : 1000;

	for (int i = 0; i < 9; i++)
	{
		if (!Grid::IsSlotEmpty(rawGrid, i)) { continue; }

		int value2 = Evaluate(!isPlaying, rawGrid, i, -beta, -alpha);

		if (isPlaying)
		{
			value = value > value2 ? value : value2;
			alpha = alpha > value ? alpha : value;

			if (alpha >= beta) { break; }
		}
		else
		{
			value = value < value2 ? value : value2;
			beta = beta < value ? beta : value;

			if (alpha >= beta) { break; }
		}
	}

	return value;
}

