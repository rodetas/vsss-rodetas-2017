#ifndef MOVIMENTATION_H_
#define MOVIMENTATION_H_

#include "../Header.h"
#include "../utils/Commons.h"
#include "../utils/Robot.h"

using namespace rodetas;

class Movimentation {

private:
	Robot robot;

    Point image;
    Command movements;

	float maxPwm;
	float powerFactor;
    float curveFactor;

	float distance_robot_ball;
    float distance_robot_destination;
    float angle_robot_destination;
    float distance_ball_destination;
    float sinAngle_robot_destination;
    float cosAngle_robot_destination;
	float cos_robot_ball;

public:

	/**
	 *
	 */
	Movimentation();

	/**
	 *
	 */
	Command movePlayers(Point);

	/**
	 *
	 */
	Pwm PWMCorrection(Point);

	/**
	 *
	 */
	void updateCalculus(rodetas::Object, rodetas::Object, Point);

	void stop();
	void turnLeft(int, int);
	void turnRight(int, int);

	void setPotencyFactor(float);
	void setCurveFactor(float);
	void setPwm(Point, char);

	void setRobot(Robot);

	Command getMovement();

};
#endif