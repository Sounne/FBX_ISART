#ifndef __DEVICE_H_INCLUDED
#define __DEVICE_H_INCLUDED

#include <memory>

#include "SFML/Window.hpp"
#include "Driver.h"
#include "FBXLoader.h"
#include "GLShader.h"

class Device
{
public:
	Device();
	~Device();

	void Init();
	void CreateSFMLWindow(uint16_t size_x, uint16_t size_y, const sf::String& title);
	void CloseSFMLWindow();
	void Run();
	void Shutdown();

private:
	std::unique_ptr<sf::Window> _window;
	std::unique_ptr<FBXLoader> _fbxloader;
	std::unique_ptr<Driver> _driver;
	std::unique_ptr<GLShader> _basic_program;
	bool _running;
};

#endif // !__DEVICE_H_INCLUDED
