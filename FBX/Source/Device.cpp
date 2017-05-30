#include "Device.h"

Device::Device()
{
	_window = std::make_unique<SFMLWindow>();
}

Device::~Device()
{
}
