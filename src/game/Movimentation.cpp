#include "Movimentation.h"

Movimentation::Movimentation(){
	velocityPast.resize(3);
	
	maxPwm = 0;
	velocity = 0;
	powerFactor = 1.2;
	curveFactor = 1.1;
}

/*
 * calculates the basic movimentation
 */
Command Movimentation::movePlayers(Point destination){

	/* movement along the field */
	if (cosAngle_robot_destination < -0.3) {
		setPwm(destination, FORWARD_MOVE);
	
	} else if (cosAngle_robot_destination > 0.3){ 
		setPwm(destination, BACK_MOVE);	

	} else {
		if (sinAngle_robot_destination > 0) {
			turnRight(30, 30);
	    } else {
			turnLeft(30, 30);
	    }
	}

	return movements;
}

void Movimentation::setPwm(Point destination, char direction){

	Pwm pwms = PWMCorrection(destination);

	int pwm1 = pwms.first;
	int pwm2 = pwms.second;

	if (pwm1 > 255) pwm1 = 255;
	if (pwm2 > 255) pwm2 = 255;

	// limitador de potencia - aceleracao progressiva( 30 e a velocidade maxima )
	maxPwm = (1.0 - float(velocity) / 30);	

	// 3 uma constante empirica de amortecimento
	// ??????
	float pwm1Correction = maxPwm * pwm1 / 2;
	float pwm2Correction = maxPwm * pwm2 / 2;

	if (pwm1Correction < 0){
		pwm1Correction = 0;
	}

	if (pwm2Correction < 0){
		pwm2Correction = 0;
	}

	if (velocity > 30) {
		pwm2Correction = pwm1Correction = 0;
	}
	
	movements.direcao = direction;
	movements.pwm1 = pwm1 - pwm1Correction;
	movements.pwm2 = pwm2 - pwm2Correction;
}

/*
 * Correct robot pwm to follow the destination
 */
Pwm Movimentation::PWMCorrection(Point destination){

	int standardPower = 100;

	int basePower = standardPower * powerFactor;
	int correctionPower = (standardPower/3) * sinAngle_robot_destination * curveFactor;
	int pwmMotor1 = (basePower - correctionPower);
	int pwmMotor2 = (basePower + correctionPower);

	return make_pair(pwmMotor1, pwmMotor2);
}

void Movimentation::turnLeft(int pwm1, int pwm2){
	movements.direcao = LEFT_MOVE;
	movements.pwm1 = pwm1;
	movements.pwm2 = pwm2;
}

void Movimentation::turnRight(int pwm1, int pwm2){
	movements.direcao = RIGHT_MOVE;
	movements.pwm1 = pwm1;
	movements.pwm2 = pwm2;
}

void Movimentation::stop(){
	movements.direcao = STOPPED_MOVE;
	movements.pwm1 = 0;
	movements.pwm2 = 0;
}

float Movimentation::calculateSpeed(){
	if (robot.x != 0 && robot.y != 0) {
		velocityPast[2] = velocityPast[1];
		velocityPast[1] = velocityPast[0];
		velocityPast[0] = robot;
	}

	velocity = distance(velocityPast[0], velocityPast[2]);

	return velocity;
}

void Movimentation::updateCalculus(rodetas::Object robot, rodetas::Object ball, Point destination){
	distance_robot_destination = distance(robot, destination);
    distance_ball_destination = distance(ball, destination);
    distance_robot_ball = distance(robot, ball);

	angle_robot_destination = calcAngle(destination, robot);
    sinAngle_robot_destination = calcSen(destination, robot);
    cosAngle_robot_destination = calcCos(destination, robot);

	cos_robot_ball = calcCos(ball, robot);
}

void Movimentation::setRobot(rodetas::Object obj){
	robot = obj;
}

Command Movimentation::getMovement(){
	std::lock_guard<std::mutex> lock(mutex);   
	return movements;
}

void Movimentation::setPotencyFactor(float power){
	std::lock_guard<std::mutex> lock(mutex);   
	this->powerFactor = power;
}

void Movimentation::setCurveFactor(float curve){
	std::lock_guard<std::mutex> lock(mutex);   
	this->curveFactor = curve;
}

void Movimentation::setImage(Point p){
	std::lock_guard<std::mutex> lock(mutex);   
	this->image = p;
}

float Movimentation::getDistanceRobotBall(){
	std::lock_guard<std::mutex> lock(mutex);   
	return distance_robot_ball;
}
    
float Movimentation::getDistanceRobotDestination(){
	std::lock_guard<std::mutex> lock(mutex);   
	return distance_ball_destination;
}

float Movimentation::getAngleRobotDestination(){
	std::lock_guard<std::mutex> lock(mutex);   
	return angle_robot_destination;
}

float Movimentation::getDistanceBallDestination(){
	std::lock_guard<std::mutex> lock(mutex);   
	return distance_ball_destination;
}

float Movimentation::getSinAngleRobotDestination(){
	std::lock_guard<std::mutex> lock(mutex);   
	return angle_robot_destination;
}

float Movimentation::getCosAngleRobotDestination(){
	std::lock_guard<std::mutex> lock(mutex);   
	return cosAngle_robot_destination;
}

float Movimentation::getCosRobotBall(){
	std::lock_guard<std::mutex> lock(mutex);   
	return cos_robot_ball;
}