#include "GameServer.hpp"
#include "Plays.hpp"
#include "GameResult.hpp"

#include <cstdint>

GameServer* GameServer::instance = nullptr;

GameServer::GameServer()
{
	instance = this;
	gameGrid = new Grid(true);
}
GameServer::~GameServer()
{
	connectionThread->join();
	delete connectionThread;
}

void GameServer::Run()
{
	connectionThread = new std::thread(
		[&]() {
			serverSocket.Run();
		}
	);

	while (serverSocket.GetConnectedClientsCount() < 1); // await one player

	while (serverSocket.GetConnectedClientsCount() > 0) // while at least one player is connected keep the server alive
	{
		while (serverSocket.GetConnectedClientsCount() < 2); // await players

		int startupPacket[sizeof(int) * 4]
		{
			(int)GameResult::None,
			(int)Plays::InvalidPlay,
			true,
			true,
		};

		serverSocket.Send(
			reinterpret_cast<char*>(startupPacket),
			SerializationHeaders::PlayResult, sizeof(int) * 4,
			PacketSendTarget::Client0
		);

		while (serverSocket.GetConnectedClientsCount() == 2); // await !players
	}
	
}

void GameServer::ParsePlay(int play, int returnBuffer[4], int clientNumber)
{
	// returnBuffer[3] = (int)!(bool)instance->playerTurn; // my sorrow is immeasurable
	returnBuffer[3] = instance->playerTurn == 0 ? 1 : 0;
	bool validPlay = instance->CheckPlay(play, clientNumber);

	if (validPlay)
	{
		if (Grid::CheckWin())
		{
			returnBuffer[0] = ((int)GameResult::PlayerOneWon + clientNumber);
			// bc both win result are in a row and <clientNumber> is 0 or 1 so the addition corrects the statement
			// im dogshit at explaining things but I swear it makes sense
		}
		else if (Grid::CheckDraw())
		{
			returnBuffer[0] = (int)GameResult::Draw;
		}
		else
		{
			returnBuffer[0] = (int)GameResult::None;
		}

		returnBuffer[1] = play;
		returnBuffer[2] = false; // lowkey despise implicit casting
	}
	else
	{
		returnBuffer[0] = (int)GameResult::None;
		returnBuffer[1] = (int)Plays::InvalidPlay;
		returnBuffer[2] = true; // lowkey hate implicit casting
	}
}

bool GameServer::CheckPlay(int play, int clientNumber)
{
	if (clientNumber != playerTurn) { return false; }

	if (!Grid::IsSlotEmpty(play)) { return false; }

	//playerTurn = (int)!(bool)playerTurn; // was pretty :(
	playerTurn = playerTurn == 0 ? 1 : 0;
	Grid::Place(play, (bool)clientNumber);
	
	return true;
}