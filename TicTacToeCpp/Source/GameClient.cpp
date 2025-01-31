#include "GameClient.hpp"
#include "ClientSocket.hpp"
#include "Grid.hpp"
#include "Window.hpp"
#include "SpritesData.hpp"
#include "Plays.hpp"
#include "GameResult.hpp"


GameClient* GameClient::instance = nullptr;

GameClient::GameClient(const std::string& userName_)
{
	instance = this;
	userName = userName_;
	window = new Window(600, 600, "Tic");
	grid = new Grid(false);
	clientSocket = new ClientSocket(userName_);
}
GameClient::~GameClient()
{
	delete window;

	delete clientSocket;
}

void GameClient::Run()
{
	clientSocket->Run();

	while (shouldRun)
	{
		Play();
		Render();
	}
}

void GameClient::Play()
{
	int play = GatherInput();

	if (play == QUIT)
	{
		int data = -1;
		clientSocket->Send(
			reinterpret_cast<char*>(&data),
			SerializationHeaders::ConnectionEvent,
			sizeof(int)
		);

		shouldRun = false;
		return;
	}

	if (play != INVALID_PLAY && canPlay)
	{
		clientSocket->Send(reinterpret_cast<char*>(&play), SerializationHeaders::Play, sizeof(int));
	}
}

void GameClient::Render()
{
	window->RenderWindow->clear(sf::Color::Black);
	grid->Render(window->RenderWindow);
	window->RenderWindow->display();
}

int GameClient::GatherInput() const
{
	int validatedInput = INVALID_PLAY;
	int rawInput = INVALID_PLAY;

	sf::Event event;
	sf::Vector2i mousePos;
	int row;
	int column;
	while (window->RenderWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->RenderWindow->close();
			return QUIT;

		case sf::Event::MouseButtonPressed:
			if (event.key.code != sf::Mouse::Button::Left) { break; }

			mousePos = sf::Mouse::getPosition(*window->RenderWindow);

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

void GameClient::HandlePlayResult(const int gameResult, const int play, const bool canPlay, const int symbol)
{
	switch ((GameResult)gameResult)
	{
	case GameResult::PlayerOneWon:
		std::cout << "\r" << "Player 1 won\n" << instance->userName << ": ";
		break;

	case GameResult::Draw:
		std::cout << "\r" << "Draw\n" << instance->userName << ": ";
		break;

	case GameResult::PlayerTwoWon:
		std::cout << "\r" << "Player 2 won\n" << instance->userName << ": ";
		break;

	case GameResult::None:
	default:
		break;
	}

	if ((Plays)play != Plays::InvalidPlay)
	{
		Grid::Place(play, symbol);
	}

	instance->canPlay = canPlay;
}