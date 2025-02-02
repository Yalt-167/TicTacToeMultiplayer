#include "GameServer.hpp"
#include "GameClient.hpp"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "No startup args given" << std::endl;
		throw std::exception("Don t ever waste my time like this");
	}

	const bool isServer = strcmp(argv[1], "Server") == 0;
	
	ShowWindow(GetConsoleWindow(), static_cast<int>(isServer));

	Game* game = (isServer ? (Game*)new GameServer() : (Game*)new GameClient());

	game->Run();

	delete game;
}