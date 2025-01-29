#include "GameClient.hpp"


GameClient::GameClient(const std::string& userName_)
{
	userName = userName_;
	window = new Window(600, 600, "Tic");
	clientSocket = new ClientSocket(userName_, this);
	canPlay = clientSocket->ClientIndex() == 0;
}

GameClient::~GameClient()
{
	delete window;

	delete clientSocket;
}

void GameClient::Run()
{
	clientSocket->Run();

	while (true)
	{
		Play();
		Render();
	}
}

void GameClient::Render()
{
	window->RenderWindow->clear(sf::Color::Black);
	grid.Render(window->RenderWindow);
	window->RenderWindow->display();
}

void GameClient::Play()
{
	int play = GatherInput(&grid, window->RenderWindow);
	//std::cout << "Play: " << play << std::endl;
	if (canPlay && play != INVALID_PLAY)
	{
		grid.Place(play, clientSocket->ClientIndex());
		clientSocket->Send(reinterpret_cast<char*>(&play), SerializationHeaders::Play);
		canPlay = false;
	}
}

int GameClient::GatherInput(Grid* grid, sf::RenderWindow* renderWindow)
{
	int validatedInput = -1;
	int rawInput = -1;

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

void GameClient::AllowPlay() {
	canPlay = true;
}

void GameClient::PlaceOnGrid(int slot, bool isO) {
	grid.Place(slot, isO);
}