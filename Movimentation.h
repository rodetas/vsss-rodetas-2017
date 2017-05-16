#ifndef MOVIMENTATION
#define MOVIMENTATION

#include "Header.h"
#include "utils/Structs.h"

using namespace rodetas;

class Movimentation{

private:
	vector<rodetas::Object> objects;
	vector<rodetas::Object> velocityPast;
	rodetas::Object robot;

    Point image;
    Command movements;

    int velocity;
	float maxPwm;
	float powerFactor;
    float curveFactor;

	float distance_robot_ball;
    float distance_robot_destination;
    float angle_robot_destination;
    float distance_ball_destination;
    float sinAngle_robot_destination;
    float cosAngle_robot_destination;

public:
	Movimentation();

	Command movePlayers(Point);
	Pwm PWMCorrection(Point);

	float calculateSpeed();
	void updateCalculus(rodetas::Object, Point);

	void turnLeft(int, int);
	void turnRight(int, int);
	void stop();

	void setPower(float);
	void setPowerCurve(float);
	void setPwm(Point, char);
	void setObjects(vector<rodetas::Object>);
	void setImage(Point);

	Command getMovement();
};
#endif