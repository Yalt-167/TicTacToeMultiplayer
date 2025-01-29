#include "Game.hpp"
#include "Player.hpp"


Game::Game(bool isServer_)
{
	isServer = isServer_;

	if (isServer)
	{
		return;
	}
	grid = new Grid(true);
	window = new Window(600, 600, "Tic");
}


Game::~Game()
{
	delete grid;

	delete window;
}

void Game::Run()
{
	if (!isServer) { Render(); } 

	while (playing)
	{
		Play();
		if (!isServer)
		{
			Render();
		}
		VerifyWin();
	}
}

void Game::Play()
{
	grid->Place(players[(int)currentPlayerIsO].GatherInput(*grid, window->RenderWindow), currentPlayerIsO);
	turns++;
}

void Game::VerifyWin()
{
	if (grid->CheckWin())
	{
		std::cout << (currentPlayerIsO ? 'O' : 'X') << " has won" << std::endl;
		Reset();
	}

	if (turns == 9)
	{
		std::cout << "Its a Draw" << std::endl;
		Reset();
	}
		
	currentPlayerIsO = !currentPlayerIsO;
}

void Game::Render()
{
	window->RenderWindow->clear(sf::Color::Black);
	grid->Render(window->RenderWindow);
	window->Rename(currentPlayerIsO ? "Tic" : "Tac");
	window->RenderWindow->display();
}

void Game::Reset()
{
	turns = 0;

	grid->Clear();

	Render();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
