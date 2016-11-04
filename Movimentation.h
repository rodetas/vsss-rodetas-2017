#ifndef MOVIMENTATION
#define MOVIMENTATION

#include "Header.h"
#include "utils/Structs.h"

using namespace rod;

class Movimentation{

private:
	vector<rod::Object> objects;
	rod::Object robot;

	vector<float> maxPwm;
	vector<rod::Object> velocityPast1;
    vector<rod::Object> velocityPast2;
    vector<rod::Object> velocityPast3;
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

	void updateCalculus(rod::Object, Point);

	void turnLeft(int, int);
	void turnRight(int, int);
	void stop();

	void setPower(float);
	void setPowerCurve(float);
	void setPwm(Point, string);
	void setRobot(rod::Object);
	void setObjects(vector<rod::Object>);
	void setImage(Point);
	void setCurveStrategy(int);
	void setNumbers(int, int, int);

	vector<Command> getMovements();
};

#endif