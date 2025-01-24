#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/window.hpp>
#include <SFML/system.hpp>

#include "Grid.hpp"
#include "Game.hpp"

#define SKIP_SFML_STUFF
constexpr auto HUMAN_PLAYER = true;
constexpr auto DO_CLEAR_CONSOLE = true;

int main(int, char**)
{
#ifdef SKIP_SFML_STUFF  
    Game(HUMAN_PLAYER, !HUMAN_PLAYER, DO_CLEAR_CONSOLE).Run();
#else
    sf::RenderWindow window(sf::VideoMode(800, 800), "Window Title");
    auto shapePtr = new sf::CircleShape(200.f, 100);
    shapePtr->setFillColor(sf::Color(204, 77, 5)); // Color circle
    shapePtr->setPosition(200, 200); // Center circle

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(18, 33, 43)); // Color background
        window.draw(*shapePtr);
        window.display();

    }
#endif
}
