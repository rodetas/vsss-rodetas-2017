#include "Movimentation.h"

Movimentation::Movimentation(){
	velocityPast1.resize(3);
	velocityPast2.resize(3);
	velocityPast3.resize(3);
	velocity.resize(3);
	maxPwm.resize(3);

	objects.resize(7);
	movements.resize(3);

	powerFactor = 1.4;
	curveFactor = 1.0;
	curveStrategy = 1;
}

/*
 * calculates the basic movimentation
 */

Command Movimentation::movePlayers(Point destination, bool corner, Information& information){

	/* movement along the field */
	if (cosAngle_robot_destination < -0.3) {

	 	information.strategy = "Attack strategy:  Forward Move ";
		setPwm(destination, FORWARD_MOVE);
	
	} else if (cosAngle_robot_destination > 0.3){ 

		information.strategy = "Attack strategy:  Back Move ";
		setPwm(destination, BACK_MOVE);	


	} else {
		if (sinAngle_robot_destination > 0) {
			information.strategy = "Attack strategy:  Turn Right ";
			if (corner) {
				turnRight(100, 100);
			} else {
				turnRight(30, 30);
			}

	    } else {
			information.strategy = "Attack strategy:  Turn Left ";
			if (corner) {	
				turnLeft(100, 100);
			} else {
				turnRight(30, 30);
			}
	    }
	}

	Command movement;
	movement.direcao = movements[robot.id].direcao;
	movement.pwm1 = movements[robot.id].pwm1;
	movement.pwm2 = movements[robot.id].pwm2;

	return movement;
}

void Movimentation::setPwm(Point destination, char direction){

	Pwm pwms = PWMCorrection(destination);

	int pwm1 = pwms.first;
	int pwm2 = pwms.second;

	if (pwm1 > 255) pwm1 = 255;
	if (pwm2 > 255) pwm2 = 255;

	// limitador de potencia - aceleracao progressiva(30 e a velocidade maxima)
	maxPwm[robot.id] = (1.0 - float(velocity[robot.id])/30);	

	// 3 uma constante empirica de amortecimento
	float pwm1Correction = maxPwm[robot.id]*pwm1/1.5;
	float pwm2Correction = maxPwm[robot.id]*pwm2/1.5;

	if (pwm1Correction < 0){
		pwm1Correction = 0;
	}

	if (pwm2Correction < 0){
		pwm2Correction = 0;
	}

	if (velocity[robot.id] > 30) {
		pwm2Correction = pwm1Correction = 0;
	}
	
	movements[robot.id].direcao = direction;
	movements[robot.id].pwm1 = pwm1 - pwm1Correction;
	movements[robot.id].pwm2 = pwm2 - pwm2Correction;
}

/*
 * Correct robot pwm to follow the destination
 */
Pwm Movimentation::PWMCorrection(Point destination){

	float distanceFactor = 0;

	int basePower = 0;
	int standardPower = 100;
	int correctionPower = 0;

	int pwmMotor1, pwmMotor2;

	if(distance_robot_destination < 120 && distance_robot_destination > 50 && distance_ball_destination > 100){
		distanceFactor = distance_robot_destination/80.0;
	} else if(distance_robot_destination < 50 && distance_ball_destination > 70 && robot.id == defenseNumber) {
		distanceFactor = 0;
	//} else if(distance_robot_destination < 45 && distance_ball_destination > 70 &&  robot.id == goalNumber) {
	} else if(destination.x + 100 > robot.x && destination.x - 100 < robot.x && abs(robot.y - destination.y ) < 60){
		distanceFactor = 0;
	} else {
		distanceFactor = 1;
	}
	distanceFactor = 1;

	basePower = standardPower * powerFactor;
	correctionPower = (standardPower/2) * sinAngle_robot_destination * curveFactor * curveStrategy;
	pwmMotor1 = (basePower-correctionPower)*distanceFactor;
	pwmMotor2 = (basePower+correctionPower)*distanceFactor;

	curveStrategy = 1;

	return make_pair(pwmMotor1, pwmMotor2);
}

void Movimentation::turnLeft(int pwm1, int pwm2){
	movements[robot.id].direcao = LEFT_MOVE;
	movements[robot.id].pwm1 = pwm1;
	movements[robot.id].pwm2 = pwm2;
}

void Movimentation::turnRight(int pwm1, int pwm2){
	movements[robot.id].direcao = RIGHT_MOVE;
	movements[robot.id].pwm1 = pwm1;
	movements[robot.id].pwm2 = pwm2;
}

void Movimentation::stop(){
	movements[robot.id].direcao = STOPPED_MOVE;
	movements[robot.id].pwm1 = 0;
	movements[robot.id].pwm2 = 0;
}

float Movimentation::calculateSpeed(){

	if (robot.x != 0 && robot.y != 0) {
		velocityPast3[robot.id] = velocityPast2[robot.id];
		velocityPast2[robot.id] = velocityPast1[robot.id];
		velocityPast1[robot.id] = robot;
	}

	velocity[robot.id] = distance(velocityPast1[robot.id], velocityPast3[robot.id]);

	return velocity[robot.id];
}

void Movimentation::updateCalculus(rodetas::Object robot, Point destination){

	distance_robot_destination = distance(robot, destination);
    distance_ball_destination = distance(objects[GRAPHICBALL], destination);
    distance_robot_ball = distance(robot, objects[GRAPHICBALL]);

	angle_robot_destination = calcAngle(destination, robot);
    sinAngle_robot_destination = calcSen(destination, robot);
    cosAngle_robot_destination = calcCos(destination, robot);
}

void Movimentation::setObjects(vector<rodetas::Object> vec){
	this->objects = vec;
}

void Movimentation::setRobot(rodetas::Object r){
	this->robot = r;
}

vector<Command> Movimentation::getMovements(){
	return movements;
}

void Movimentation::setPower(float power){
	this->powerFactor = power;
}

void Movimentation::setPowerCurve(float curve){
	this->curveFactor = curve;
}

void Movimentation::setImage(Point p){
	this->image = p;
}

void Movimentation::setCurveStrategy(int p){
	this->curveStrategy = p;
}

void Movimentation::setNumbers(int attack, int defense, int goal){
	this->attackNumber = attack;
	this->defenseNumber = defense;
	this->goalNumber = goal;
}
