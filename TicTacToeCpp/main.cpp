#include "Grid.hpp"

#include "GameServer.hpp"
#include "GameClient.hpp"


int main(int argc, char** argv)
{
	if (argc < 2) { throw std::exception("Don t ever waste my time like this"); }

	bool isServer = strcmp(argv[1], "Server") == 0;

	std::string username;
	if (!isServer)
	{
		std::cout << "Enter your username: " << std::endl;
		std::cin >> username;
	}

	Game_* game = (isServer ? (Game_*)new GameServer() : (Game_*)new GameClient(username));
	game->Run();
}
