#pragma once

#include "ClientSocket.hpp"
#include "Window.hpp"
#include "Grid.hpp"
#include "SpritesData.hpp"

class GameClient : public Game_
{
public:
	GameClient(const std::string& userName_);
	~GameClient();

	void Run() override;
	void Render();
	void Play();
	int GatherInput(Grid* grid, sf::RenderWindow* renderWindow);
	void AllowPlay();
	void PlaceOnGrid(int slot, bool isO);

private:
	ClientSocket* clientSocket;
	std::string userName;

	Window* window = nullptr;
	Grid grid;
	bool canPlay = false;
	const int INVALID_PLAY = -1;
};