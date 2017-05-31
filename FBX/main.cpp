#include <iostream>

#include "GL/glew.h"
#include "Device.h"

int main(int ac, char** av)
{
	Device* device = new Device();

	device->Init();

	device->Run();

	delete device;

	return EXIT_SUCCESS;
}