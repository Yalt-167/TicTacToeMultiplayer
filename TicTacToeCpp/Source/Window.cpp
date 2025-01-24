#include "Window.hpp"
#include "SpritesData.hpp"


Window::Window(int windowWidth, int windowHeight, const std::string& windowName)
{
	RenderWindow = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), windowName);
}

Window::~Window()
{
	delete RenderWindow;
}

void Window::Rename(const std::string& newName)
{
	RenderWindow->setTitle(newName);
}
