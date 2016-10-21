#ifndef MOVIMENTATION
#define MOVIMENTATION

#include "Header.h"
#include "utils/Structs.h"

using namespace rod;

class Movimentation{

private:
	vector<Object> objects;
	Object robot;

	vector<float> maxPwm;
	vector<Object> velocityPast1;
    vector<Object> velocityPast2;
    vector<Object> velocityPast3;
    vector<int> velocity;

    vector<Command> movements;

	float distance_robot_ball;
    float distance_robot_destination;
    float angle_robot_destination;
    float distance_ball_destination;
    float sinAngle_robot_destination;
    float cosAngle_robot_destination;

    float powerFactor;
    float curveFactor;

    float curveStrategy;

    Point image;

    int attackNumber;
    int defenseNumber;
    int goalNumber;

public:

	Movimentation();

	Command movePlayers(Point, bool,Information&);
	Pwm PWMCorrection(Point);

	float calculateSpeed();

	void updateCalculus(Object, Point);

	void turnLeft(int, int);
	void turnRight(int, int);
	void stop();

	void setPower(float);
	void setPowerCurve(float);
	void setPwm(Point, string);
	void setRobot(Object);
	void setObjects(vector<Object>);
	void setImage(Point);
	void setCurveStrategy(int);
	void setNumbers(int, int, int);

	vector<Command> getMovements();
};

#endif