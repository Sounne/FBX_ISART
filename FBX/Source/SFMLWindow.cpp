#include "SFMLWindow.h"

SFMLWindow::SFMLWindow()
{
	_size = sf::Vector2u(800, 600);
	_window.create(sf::VideoMode(_size.x, _size.y), "FBX Loader", sf::Style::Default);
}

SFMLWindow::~SFMLWindow()
{
}

void SFMLWindow::Update()
{
	_window.display();
}

void SFMLWindow::Exit()
{
	_window.close();
}
