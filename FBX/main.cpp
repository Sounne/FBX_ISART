#include <iostream>

#include "Device.h"

int main(int* ac, char** av)
{
	Device* device = new Device();

	delete device;

	return EXIT_SUCCESS;
}