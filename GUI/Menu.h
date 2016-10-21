#ifndef MENU_H_
#define MENU_H_

#include "../Header.h"
#include "widgets/Button.h"

#include <SFML/Graphics.hpp>

class Menu{
private:

	vector<Button> button;
	sf::RectangleShape background;
	sf::Texture texture;

public:
	Menu();
	~Menu();
	int menu();
	void drawComponents();

};
#endif