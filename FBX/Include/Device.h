#ifndef __DEVICE_H_INCLUDED
#define __DEVICE_H_INCLUDED

#include <memory>

#include "SFML/Window.hpp"
#include "FBXLoader.h"

class Device
{
public:
	Device();
	~Device();

	void CreateWindow(uint16_t size_x, uint16_t size_y, const sf::String& title);
	void CloseWindow();
	void Run();

private:
	std::unique_ptr<sf::Window> _window;
	bool _running;
};

#endif // !__DEVICE_H_INCLUDED
