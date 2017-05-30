#ifndef __SFMLWINDOW_H_INCLUDED
#define __SFMLWINDOW_H_INCLUDED

#include "SFML/Window.hpp"

class SFMLWindow
{
public:
	SFMLWindow();
	~SFMLWindow();

	void Update();
	void Exit();

private:
	sf::Window _window;
	sf::Vector2u _size;
};

#endif // !__SFMLWINDOW_H_INCLUDED
