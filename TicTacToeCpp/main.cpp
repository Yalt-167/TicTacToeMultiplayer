#include "GameServer.hpp"
#include "GameClient.hpp"

#define DO_HIDE_CLIENT_CONSOLE
#define LOG_EVERY_SINGLE_PACKET true

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "No startup args given" << std::endl;
		throw std::exception("Don t ever waste my time like this");
	}

	const bool isServer = strcmp(argv[1], "Server") == 0;

#ifdef DO_HIDE_CLIENT_CONSOLE
	ShowWindow(GetConsoleWindow(), static_cast<int>(isServer));
#endif

	Game* game = (isServer ? (Game*)new GameServer(LOG_EVERY_SINGLE_PACKET) : (Game*)new GameClient());

	game->Run();

	delete game;
}