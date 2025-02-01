#include "GameServer.hpp"
#include "Plays.hpp"
#include "GameResult.hpp"
#include "PacketSendTarget.hpp"
#include "Grid.hpp"

GameServer* GameServer::instance = nullptr;

GameServer::GameServer()
{
	instance = this;
	gameGrid = new Grid(true);

	startupPacket[0] = static_cast<int>(GameResult::None);
	startupPacket[1] = static_cast<int>(Plays::InvalidPlay);
	startupPacket[3] = static_cast<int>(true);

	ZeroMemory(gridState, 9); // for the sake of intellisense happy
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
		while (serverSocket.GetConnectedClientsCount() < 2); // await players to start the game

		serverSocket.Send(
			Grid::Serialize(gridState),
			SerializationHeaders::CatchupPacket,
			sizeof(char) * 9,
			PacketSendTarget::Broadcast
		);

		for (int i = 0; i < 2; i++) // loop over clients
		{
			startupPacket[2] = static_cast<int>(i == playerTurn); // wether that client has the turn
			serverSocket.Send(
				reinterpret_cast<char*>(startupPacket),
				SerializationHeaders::PlayResult, sizeof(int) * 4,
				static_cast<PacketSendTarget>(i)
			);
		}

		while (serverSocket.GetConnectedClientsCount() == 2); // await !players
	}
}

void GameServer::ParsePlay(const int play, int returnBuffer[4], const int clientNumber)
{
	/*
	if we branched into that method it means that
	returnBuffer is basically structured as such:
	{
	<int: game state after the play>
	<int: the play itself>
	<int: wether u can play>,
	<int: who played>
	}
	*/
	// lets set those for clarity and avoid magic values:
	static constexpr const int gameStateAfterPlay = 0;
	static constexpr const int playItself = 1;
	static constexpr const int canPlay = 2;
	static constexpr const int whoPlayed = 3;

	// returnBuffer[3] = (int)!(bool)instance->playerTurn; // my sorrow is immeasurable
	returnBuffer[whoPlayed] = instance->playerTurn == 0 ? 1 : 0;
	bool validPlay = instance->CheckPlay(play, clientNumber);

	if (validPlay)
	{
		if (Grid::CheckWin())
		{
			returnBuffer[gameStateAfterPlay] = static_cast<int>(GameResult::PlayerZeroWon) + clientNumber;
			// bc both win result are in a row and <clientNumber> is 0 or 1 so the addition corrects the statement
			// im dogshit at explaining things but I swear it makes sense
		}
		else
		{
			returnBuffer[gameStateAfterPlay] = static_cast<int>(Grid::CheckDraw() ? GameResult::Draw : GameResult::None);
		}

		returnBuffer[playItself] = play;
		returnBuffer[canPlay] = static_cast<int>(false); // lowkey despise implicit casting
	}
	else
	{
		returnBuffer[gameStateAfterPlay] = static_cast<int>(GameResult::None); // ur "efforts" amounted to nothing (again)
		returnBuffer[playItself] = static_cast<int>(Plays::InvalidPlay); // u ve somehow managed not to understand how TicTacToe works
		returnBuffer[canPlay] = static_cast<int>(true); // lowkey hate implicit casting
	}
}

bool GameServer::CheckPlay(const int play, const int clientNumber)
{
	if (clientNumber != playerTurn) { return false; }

	if (!Grid::IsSlotEmpty(play)) { return false; }

	//playerTurn = (int)!(bool)playerTurn; // was pretty :(
	playerTurn = playerTurn == 0 ? 1 : 0;
	Grid::Place(play, static_cast<bool>(clientNumber));
	
	return true;
}