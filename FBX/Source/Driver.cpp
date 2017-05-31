#include "Driver.h"
#include "GL/glew.h"

Driver::Driver()
{
}


Driver::~Driver()
{
}

void Driver::Clear()
{
	const float background[4] = { 1.f, 0.f, 0.f, 1.f };

	glClearColor(background[0], background[1], background[2], background[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Driver::Render()
{
}
