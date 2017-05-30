#ifndef __DEVICE_H_INCLUDED
#define __DEVICE_H_INCLUDED

#include <memory>

#include "SFMLWindow.h"

class Device
{
public:
	Device();
	~Device();

	void Run();

private:
	std::unique_ptr<SFMLWindow> _window;
};

#endif // !__DEVICE_H_INCLUDED
