#include <iostream>
#include <limits>

#include "../Include/MyAI.hpp"
#include "../Include/Tree.hpp"

MyAI::MyAI(char symbol_)
{
	symbol = symbol_;
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
	for (Node<std::vector<std::vector<char>>>* node : currentRoot->Children)
	{
		if (node->Value == currentRawGrid)
		{
			newRoot = node;
		}
		else
		{
			currentRoot->RemoveChild(node);
		}
	}

	currentRoot = newRoot;
}

int MyAI::Think(Grid& currentGrid)
{
	currentRoot = Evaluate();
	return Grid::GetDifference(currentGrid.GetRaw(), currentRoot->Value);
}



