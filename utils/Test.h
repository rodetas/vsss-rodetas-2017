#ifndef TEST_H_
#define TEST_H_

#include "../Header.h"
#include "Structs.h"
#include "OpenGL.h"
#include "Fps.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Test{

	private:
		sf::RenderWindow window;
		Point2f view;
		OpenGL opengl;

	public:
		Test();
		int loop();
};

#endif