#include "Movimentation.h"

Movimentation::Movimentation(){
	maxPwm = 0;
	// inicializacoes estao no data
//	powerFactor = 1.0f;
//	curveFactor = 1.2f;
}

/*
 * calculates the basic movimentation
 */
Command Movimentation::movePlayers(const Robot& robot){

	Point destination = robot.getTarget();
	Command command;

	/* movement along the field */
	if (robot.cosFrom(destination) < -0.3) {
		command = definePwm(robot, BACK_MOVE);
	
	} else if (robot.cosFrom(destination) > 0.3){ 
		command = definePwm(robot, FORWARD_MOVE);	

	} else {
		if (robot.sinFrom(destination) > 0) {
			command = turnRight(60, 60);
	    } else {
			command = turnLeft(60, 60);
	    }
	}

	return command;
}

Command Movimentation::checkPwm(const Command& pwm){
	Command command(pwm);

	if(pwm.pwm1 > 255) command.pwm1 = 255;
	if(pwm.pwm2 > 255) command.pwm2 = 255;
	
	if(pwm.pwm1 < 0) command.pwm1 = 0;
	if(pwm.pwm2 < 0) command.pwm2 = 0;

	return command;
}

/*
 * Correct robot pwm to follow the destination
 */
Command Movimentation::definePwm(const Robot& robot, char direction){

	int standardPower = 160;

	int basePower = standardPower * powerFactor;
	int correctionPower = (standardPower/4) * robot.sinFrom(robot.getTarget()) * curveFactor;
	int pwmMotor1 = (basePower + correctionPower);
	int pwmMotor2 = (basePower - correctionPower);

	Command verifiedPwm = checkPwm(Command(pwmMotor1, pwmMotor2, direction));

	return verifiedPwm;
}

Command Movimentation::turnLeft(int pwm1, int pwm2){
	Command command(pwm1, pwm2, LEFT_MOVE);
	return command;
}

Command Movimentation::turnRight(int pwm1, int pwm2){
	Command command(pwm1, pwm2, RIGHT_MOVE);
	return command;
}

Command Movimentation::stop(){
	Command command(0, 0, STOPPED_MOVE);
	return command;
}

Command Movimentation::getMovement(){
	return movements;
}

void Movimentation::setPotencyFactor(float power){
	this->powerFactor = power;
}

void Movimentation::setCurveFactor(float curve){
	this->curveFactor = curve;
}