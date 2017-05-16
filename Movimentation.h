#ifndef MOVIMENTATION
#define MOVIMENTATION

#include "Header.h"
#include "utils/Structs.h"

using namespace rodetas;

class Movimentation{

private:
	vector<rodetas::Object> objects;
	rodetas::Object robot;

	//vector<float> maxPwm;
	float maxPwm;

	//vector<rodetas::Object> velocityPast1;
	float velocityPast1;
    //vector<rodetas::Object> velocityPast2;
    float velocityPast2;
    //vector<rodetas::Object> velocityPast3;
    float velocityPast3

    //vector<int> velocity;
    int velocity;

    //vector<Command> movements;
    Command *movements = new Command();

	float distance_robot_ball;
    float distance_robot_destination;
    float angle_robot_destination;
    float distance_ball_destination;
    float sinAngle_robot_destination;
    float cosAngle_robot_destination;

    float powerFactor;
    float curveFactor;

    Point image;

    int attackNumber;
    int defenseNumber;
    int goalNumber;

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
	void setRobot(rodetas::Object);
	void setObjects(vector<rodetas::Object>);
	void setImage(Point);

	//seters e geters novos
	void setMaxPwm(float);

	void setVelocityPast1(float);
	void setVelocityPast2(float);
	void setVelocityPast3(float);
	void setVelocity(int);

	float getMaxPwm();

	float getVelocityPast1();
	float getVelocityPast2();
	float getVelocityPast3();
	int getVelocity();


	//vector<Command> getMovements();
};

#endif
