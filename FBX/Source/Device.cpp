#include <iostream>

#include "Device.h"
#include "GL/glew.h"

Device::Device()
{
	_window = std::make_unique<sf::Window>();
	_driver = std::make_unique<Driver>();
	_basic_program = std::make_unique<GLShader>();
	_fbxloader = std::make_unique<FBXLoader>();
	_fbxloader->SetProgram(_basic_program.get());
	CreateSFMLWindow(800, 600, "FBX Loader");
	_fbxloader->Init();
}

Device::~Device()
{
	CloseSFMLWindow();
}

void Device::Init()
{
	glewInit();
	glEnable(GL_FRAMEBUFFER_SRGB);
	_basic_program->LoadShader(GL_FRAGMENT_SHADER, "Shaders/DirectionalLight.fs");
	_basic_program->LoadShader(GL_VERTEX_SHADER, "Shaders/DirectionalLight.vs");
	_basic_program->Create();

	_fbxloader->Init();
	_fbxloader->LoadScene("data/ironman/ironman.fbx");
}

void Device::CreateSFMLWindow(uint16_t size_x, uint16_t size_y, const sf::String & title)
{
	_window->create(sf::VideoMode(size_x, size_y), title, sf::Style::Default);
	_running = true;
}

void Device::CloseSFMLWindow()
{
	_window->close();
}

void Device::Run()
{
	while (_running)
	{
		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				_window->close();
		}

		_window->setActive();

		if (_driver)
		{
			_driver->Clear();
			//_driver->Render();
		}

		if (_fbxloader)
		{
			_fbxloader->Render();
		}

		if (_window)
			_window->display();
	}
}

void Device::Shutdown()
{
}
