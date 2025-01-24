

#include "Grid.hpp"
#include "Game.hpp"

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
	

    Game(isServer).Run();
}
