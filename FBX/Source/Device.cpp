#include "Device.h"
#include "GL/glew.h"

Device::Device()
{
	_window = std::make_unique<sf::Window>();
	_fbxloader = std::make_unique<FBXLoader>();
	_basic_program = std::make_unique<GLShader>();
	CreateWindow(800, 600, "FBX Loader");

	_fbxloader->Init();
}

Device::~Device()
{
	CloseWindow();
}

void Device::Init()
{
	glewInit();
	glEnable(GL_FRAMEBUFFER_SRGB);
	_basic_program->LoadShader(GL_FRAGMENT_SHADER, "Shaders/DirectionalLight.fs");
	//_basic_program->LoadShader(GL_VERTEX_SHADER, "Shaders/DirectionalLight.vs");
	//_basic_program->Create();

	//_fbxloader->Init();
	//_fbxloader->LoadScene("data/ironman/ironman.fbx");
}

void Device::CreateWindow(uint16_t size_x, uint16_t size_y, const sf::String & title)
{
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
