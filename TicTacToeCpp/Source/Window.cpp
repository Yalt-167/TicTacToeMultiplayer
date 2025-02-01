#include "Window.hpp"
#include "SpritesData.hpp"


Window::Window(const int windowWidth, const int windowHeight, const std::string& windowName)
{
	RenderWindow = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), windowName, sf::Style::Titlebar | sf::Style::Close);
}

Window::~Window()
{
	delete RenderWindow;
}

void Window::Rename(const std::string& newName)
{
	RenderWindow->setTitle(newName);
}
