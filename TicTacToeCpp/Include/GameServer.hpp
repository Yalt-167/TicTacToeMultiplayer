#pragma once

#include "Grid.hpp"
#include "ServerSocket.hpp"


class GameServer : public Game_
{
public:
	GameServer()
	{
		Instance = this;
		gameGrid = new Grid(true);
	}
	~GameServer()
	{
		connectionThread->join();
		delete connectionThread;
	}

	void Run() override
	{
		connectionThread = new std::thread(
			[&]() {
				serverSocket.Run();
			}
		);

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

	int PlayerTurn = 0;
	static GameServer* Instance;
private:
	Grid* gameGrid;
	ServerSocket serverSocket;
	std::thread* connectionThread = nullptr;
};