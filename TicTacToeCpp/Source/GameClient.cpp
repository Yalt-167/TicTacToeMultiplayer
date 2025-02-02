#include "GameClient.hpp"
#include "ClientSocket.hpp"
#include "Grid.hpp"
#include "Window.hpp"
#include "SpritesData.hpp"
#include "Plays.hpp"
#include "GameResult.hpp"


GameClient* GameClient::instance = nullptr;

const std::string GameClient::youCanPlay = "Your turn";
const std::string GameClient::youCantPlay = "!Your turn";

GameClient::GameClient(const std::string& userName_)
{
	instance = this;
	userName = userName_;
	window = new Window(900, 600, "Tic");
	grid = new Grid(false);
	clientSocket = new ClientSocket(userName_, &chatMessages);
}
GameClient::~GameClient()
{
	delete window;
	delete clientSocket;

	ImGui::SFML::Shutdown();
}

void GameClient::Run()
{
	clientSocket->Run();

	ImGUI::SFML::Init(*window->RenderWindow);

	while (shouldRun)
	{
		Play();
		Chat();
		Render();
	}
}

void GameClient::Play()
{
	const int play = GatherInput();

	if (play == QUIT)
	{
		clientSocket->Send(
			reinterpret_cast<const char*>(&QUIT),
			SerializationHeaders::Disconnection,
			sizeof(int)
		);

		shouldRun = false;
		return;
	}

	if (play != INVALID_PLAY && canPlay)
	{
		clientSocket->Send(reinterpret_cast<const char*>(&play), SerializationHeaders::Play, sizeof(int));
	}
}

void GameClient::Render() const
{
	window->RenderWindow->clear(sf::Color::Black);
	grid->Render(window->RenderWindow);
	ImGui::SFML::Render(*window->RenderWindow);
	window->RenderWindow->display();
}

void GameClient::Chat() {
	ImGUI::SFML::Update(*window->RenderWindow, sf::Time());

	ImGUI::Begin("Chat Window", nullptr, ImGuiWindowFlags_NoMove);
	ImGUI::SetWindowPos(ImVec2(620, 10));

	ImGui::BeginChild("ChatBox", ImVec2(250, 520), true);

	// Display all the chat messages
	for (const auto& message : chatMessages)
	{
		ImGui::TextWrapped("%s", message.c_str());
	}

	ImGui::EndChild();

	// Input box to enter new chat message
	static char inputText[256] = "";  // Store the input message
	if (ImGui::InputText("##ChatInput", inputText, IM_ARRAYSIZE(inputText), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (strlen(inputText) > 0)
		{
			std::string msg = "[" + userName + "]: " + std::string(inputText);
			// Add the new message to the chat
			chatMessages.push_back(msg);
			clientSocket->Send(msg.c_str(), SerializationHeaders::ChatMessage, msg.length() + 1);
		}
		// Clear the input field after sending the message
		inputText[0] = '\0';
	}

	ImGUI::End();
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
		ImGUI::SFML::ProcessEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			window->RenderWindow->close();
			return QUIT;

		case sf::Event::MouseButtonPressed:
			if (event.key.code != sf::Mouse::Button::Left) { break; }

			mousePos = sf::Mouse::getPosition(*window->RenderWindow);
			if (mousePos.x > 600) { break; } // within the chat boundaries so avoid counting it as a play

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
	if (static_cast<Plays>(play) != Plays::InvalidPlay)
	{
		Grid::Place(play, symbol);
	}

	switch (static_cast<GameResult>(gameResult))
	{
	case GameResult::PlayerZeroWon:
		std::cout << "\r" << "Player 1 won\n" << instance->userName << ": ";
		Grid::Clear();
		break;

	case GameResult::Draw:
		std::cout << "\r" << "Draw\n" << instance->userName << ": ";
		Grid::Clear();
		break;

	case GameResult::PlayerOneWon:
		std::cout << "\r" << "Player 2 won\n" << instance->userName << ": ";
		Grid::Clear();
		break;

	case GameResult::None:
	default:
		break;
	}

	instance->canPlay = canPlay;

	instance->window->Rename(canPlay ? youCanPlay : youCantPlay);
}