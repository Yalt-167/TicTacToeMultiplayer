#include "GameServer.hpp"


void GameServer::Run()
{
	serverSocket.Run();

	while (serverSocket.GetConnectedClientsCount() < 2); // await players

	//serverSocket.Broadcast(SerializationHeaders::Start.c_str());

}