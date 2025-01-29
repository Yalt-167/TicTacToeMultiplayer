#pragma once

#include "ClientSocket.hpp"
#include "Window.hpp"
#include "Grid.hpp"
#include "SpritesData.hpp"

class GameClient : public Game_
{
public:
	GameClient(const std::string& userName_)
	{
		userName = userName_;
		window = new Window(600, 600, "Tic");
		grid = new Grid(false);
		clientSocket = new ClientSocket(userName_);

		
	}
	~GameClient()
	{
		delete window;

		delete clientSocket;
	}
	void Run() override
	{
		clientSocket->Run();

		while (true)
		{
			Play();
			Render();
		}
	}

	void Render()
	{
		window->RenderWindow->clear(sf::Color::Black);
		grid->Render(window->RenderWindow);
		window->RenderWindow->display();
	}
	void Play()
	{
		int play = GatherInput(grid, window->RenderWindow);
		if (play != INVALID_PLAY && CanPlay)
		{
			clientSocket->Send(reinterpret_cast<char*>(&play), SerializationHeaders::Play, sizeof(int));
		}
	}

	int GatherInput(Grid* grid, sf::RenderWindow* renderWindow) const
	{
		int validatedInput = INVALID_PLAY;
		int rawInput = INVALID_PLAY;

		sf::Event event;
		sf::Vector2i mousePos;
		int row;
		int column;
		while (renderWindow->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				renderWindow->close();
				return 0;

			case sf::Event::MouseButtonPressed:
				if (event.key.code != sf::Mouse::Button::Left) { break; }

				mousePos = sf::Mouse::getPosition(*renderWindow);

				column = mousePos.x / SpritesData::CellSize;
				row = mousePos.y / SpritesData::CellSize;

				rawInput = 3 * row + column;

				if (grid->IsSlotEmpty(rawInput))
				{
					validatedInput = rawInput;
				}
			}
		}

		return validatedInput;
	}

	static bool CanPlay;
private:
	ClientSocket* clientSocket;
	std::string userName;

	Window* window = nullptr;
	Grid* grid;
	const int INVALID_PLAY = -1;
};