#include "GameClient.hpp"
#include "ClientSocket.hpp"
#include "Grid.hpp"
#include "Window.hpp"
#include "SpritesData.hpp"
#include "Plays.hpp"
#include "GameResult.hpp"

//#define HIDE_CHAT

GameClient* GameClient::instance = nullptr;

const std::string GameClient::youCanPlay = "Your turn";
const std::string GameClient::youCantPlay = "!Your turn";

GameClient::GameClient()
{
	instance = this;
	window = new Window(
#ifdef HIDE_CHAT
		600, 
#else
		900,
#endif
		600, "TicTacToe	");
	grid = new Grid(false);
}
GameClient::~GameClient()
{
	delete window;
	delete clientSocket;

	ImGui::SFML::Shutdown();
}

void GameClient::Run()
{
	ImGUI::SFML::Init(*window->RenderWindow);

	if (PickUsername())
	{
		return;
	}

	clientSocket = new ClientSocket(userName, &chatMessages);
	clientSocket->Run();


	while (shouldRun)
	{
		Play();
		Chat();
		Render();
	}
}

bool GameClient::PickUsername()
{
	char userName_[64] = "";

	sf::Event event;
	while (userName.empty())
	{
		ImGUI::SFML::Update(*window->RenderWindow, sf::Time());
		
		while (window->RenderWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->RenderWindow->close();
				return true;
			}

			ImGUI::SFML::ProcessEvent(event);
		}

		ImGui::SetNextWindowSize(ImVec2(300, 60));
		ImGui::SetNextWindowPos(ImVec2(300, 270), ImGuiCond_Always);

		ImGui::Begin("Username Picker", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

		// shamelessly permanently borrowed Hugo s code ^^
		if (ImGui::InputText("##userNamePicker", userName_, IM_ARRAYSIZE(userName_), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (strlen(userName_) > 0)
			{
				userName = userName_;
			}
		}
	
		ImGUI::End();

		window->RenderWindow->clear(sf::Color::Black);
		//grid->Render(window->RenderWindow);
		ImGui::SFML::Render(*window->RenderWindow);
		window->RenderWindow->display();

	}

	return false;
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
			clientSocket->Send(msg.c_str(), SerializationHeaders::ChatMessage, static_cast<int>(msg.length()) + 1);
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
	switch (static_cast<GameResult>(gameResult))
	{
	case GameResult::PlayerZeroWon:
		std::cout << "\r" << "Player 1 won\n" << instance->userName << ": ";
		break;

	case GameResult::Draw:
		std::cout << "\r" << "Draw\n" << instance->userName << ": ";
		break;

	case GameResult::PlayerOneWon:
		std::cout << "\r" << "Player 2 won\n" << instance->userName << ": ";
		break;

	case GameResult::None:
	default:
		break;
	}

	if (static_cast<Plays>(play) != Plays::InvalidPlay)
	{
		Grid::Place(play, symbol);
	}

	instance->canPlay = canPlay;

	instance->window->Rename(canPlay ? youCanPlay : youCantPlay);
}