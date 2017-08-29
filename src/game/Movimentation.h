#ifndef MOVIMENTATION_H_
#define MOVIMENTATION_H_

#include "../Header.h"
#include "../utils/Commons.h"
#include "../utils/Robot.h"

using namespace rodetas;

class Movimentation {

private:

    Point image;
    Command movements;

	float maxPwm;
	float powerFactor;
    float curveFactor;

public:

	/**
	 *
	 */
	Movimentation();

	/**
	 *
	 */
	Command movePlayers(Robot);

	/**
	 *
	 */
	Command definePwm(Robot&, char);

	Command stop();
	Command turnLeft(int, int);
	Command turnRight(int, int);

	void setPotencyFactor(float);
	void setCurveFactor(float);
	Command checkPwm(const Command&);

	Command getMovement();

};
#endif