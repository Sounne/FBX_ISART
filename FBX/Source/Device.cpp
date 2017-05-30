#include "Device.h"

Device::Device()
{
	_window = std::make_unique<SFMLWindow>();
}

Device::~Device()
{
}

void Device::Run()
{
	_window.get()->Update();
}
