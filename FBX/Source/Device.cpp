#include "Device.h"

Device::Device()
{
	_running = false;
	CreateWindow(866, 600, "FBX Loader");
}

Device::~Device()
{
	CloseWindow();
}

void Device::CreateWindow(uint16_t size_x, uint16_t size_y, const sf::String & title)
{
	_window = std::make_unique<sf::Window>();
	_window->create(sf::VideoMode(size_x, size_y), title, sf::Style::Default);
	_running = true;
}

void Device::CloseWindow()
{
	_window->close();
}

void Device::Run()
{
	while (_running)
	{
		_window->display();
	}
}
