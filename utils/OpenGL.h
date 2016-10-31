#ifndef OPENGL_H_
#define OPENGL_H_

#include "Structs.h"
#include <SFML/OpenGL.hpp>

using namespace rod;

class OpenGL {

private:
	Point2f window;
	Point2f view;
	float size;	

public:
	OpenGL();
	void initializeOpenGL(Point2f, Point2f, float);
	Point2f openGLCoordinates(Point2f);
	void drawRobotOpenGL(Robot_Draw);
	void drawArrowOpenGL(Point2f, Point2f);
};
#endif