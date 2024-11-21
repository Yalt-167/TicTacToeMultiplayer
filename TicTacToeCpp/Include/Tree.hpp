#pragma once

#include <iostream>
#include <functional>
#include <vector>

template <typename T>
class Node
{
public:
	Node(T value)
	{
		Value = value;
		Parent = nullptr;
	}
	T Value;
	Node* Parent;
	std::vector<Node*> Children;
	void AddChild(Node* child)
	{
		Children.push_back(child);
		child->Parent = this;
	}
	void RemoveChild(Node* child)
	{
		child->RemoveChildren();
		Children.erase(std::remove(Children.begin(), Children.end(), child), Children.end());
		child->Parent = nullptr;
	}
	void RemoveChildren()
	{
		for (Node* child : Children)
		{
			child->RemoveChildren();
		}

		Children.clear();

	}
	void RemoveParent()
	{
		Parent = nullptr;
	}
	bool IsRoot()
	{
		return Parent == nullptr;
	}
	bool IsLeaf()
	{
		return Children.size() == 0;
	}
	void DepthFirst()
	{
		std::cout << this->Value << std::endl;

		for (Node* node : Children)
		{
			node->DepthFirst();
		}
	}
	int Evaluate(std::function<int(const std::vector<std::vector<char>>& grid, char symbolThatShouldWin_)> evaluationMethod, char symbolThatShouldWin)
	{
		if (IsLeaf())
		{
			return evaluationMethod(Value, symbolThatShouldWin);
		}

		int score = 0;
		for (Node* child : Children)
		{
			score += child->Evaluate(evaluationMethod, symbolThatShouldWin);
		}

		return score;
	}
};
