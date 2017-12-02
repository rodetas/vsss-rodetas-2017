#include "StrategyAttack.h"

StrategyAttack::StrategyAttack() : StrategyBase(){

}

Command StrategyAttack::strategy(Robot* robot, Command command){

	Command c = command;

	c = kickStrategy(c);
	c = cornerStrategy(c);
	//c = stopStrategy(c);
	//c = collisionStrategy(c);
	c = blockedStrategy(c);

	if (robot->isParallelGoal()){
		
		int halfGoal1 = rodetas::imageSize.y/2 + (rodetas::goalSize.y/2);
		int halfGoal2 = rodetas::imageSize.y/2 - (rodetas::goalSize.y/2);
		
		if ( robot->distanceFrom(data->getBall()->getPosition()) < robot->getRadius() * 1.3 && 
		!(robot->x() > imageSize.x * 0.9 && robot->y() > halfGoal1) &&
		!(robot->x() > imageSize.x * 0.9 && robot->y() < halfGoal2) ){
			
			if (robot->y() > data->getBall()->getPosition().y) {
				c = movimentation.turnRight(255, 255);
			} else {
				c = movimentation.turnLeft(255, 255);
			}
		}
	}
		
	return c;
}

Command StrategyAttack::kickStrategy(Command _command){
	Command c = _command;

	Point centerGoal = Point(imageSize.x, imageSize.y/2);
	float angle_robot_goal = calcAngle(centerGoal, robot->getPosition());

	if(angle_robot_goal < 45.0 && angle_robot_goal > -45.0 && (robot->cosFrom(centerGoal) < -0.8 || robot->cosFrom(centerGoal) > 0.8) && 
		(robot->cosFrom(data->getBall()->getPosition()) < -0.8 || robot->cosFrom(data->getBall()->getPosition()) > 0.8) &&
		robot->x() < data->getBall()->x() && robot->distanceFrom(data->getBall()->getPosition()) < robot->getRadius()*2){

		c.pwm1 *= 1.5;
		c.pwm2 *= 1.5;
	} 

	return c;
}

Command StrategyAttack::collisionStrategy(Command _command){  
	Command c = _command;

	int angle_ball = calcAngle(robot->getPosition(), data->getBall()->getPosition());
	int angle_defense = calcAngle(data->getRobot("defense").getPosition(), data->getBall()->getPosition());

	angle_ball = (angle_ball + 360) % 360; // converter para 0 - 360
	angle_defense = (angle_defense + 360) % 360;

	float sin_angle = sin(angle_ball - angle_defense);

	if ( distance(robot->getProjection(), data->getRobot("defense").getProjection() ) < robot->getRadius() * 8){

		char robotWay = c.direcao;
		char defenseWay = data->getRobot("defense").getCommand().direcao;
		if(robotWay == defenseWay){
			// os dois de frente ou de costas
			c.pwm2 = c.pwm2 * abs(sin_angle) + 0.3;

		} else if(robotWay != defenseWay){
			// os robos estao em sentidos opostos
			c.pwm1 = c.pwm1 * abs(sin_angle) + 0.3;
		}
	}

	return c;
}

Point StrategyAttack::defineTarget(Robot* robot){
    Point target;// = data->getBall()->getPosition();
	Ball* ball = data->getBall();

	target = ball->getBallProjection();

	Point centerGoal = Point(imageSize.x, imageSize.y/2);
	float angle_robot_goal = calcAngle(centerGoal, robot->getPosition());

	if(angle_robot_goal < 45.0 && angle_robot_goal > -45.0 && (robot->cosFrom(centerGoal) < -0.8 || robot->cosFrom(centerGoal) > 0.8) && 
		(robot->cosFrom(data->getBall()->getPosition()) < -0.8 || robot->cosFrom(data->getBall()->getPosition()) > 0.8) &&
		robot->x() < data->getBall()->x() && robot->distanceFrom(data->getBall()->getPosition()) < robot->getRadius()*2){

		target = centerGoal;
	} 

	Point ballProjection = ball->getBallProjection();

	if(ballProjection.x < imageSize.x*0.7 && ballProjection.x < robot->x()){
		if(ballProjection.y > imageSize.y/2){
			target.y = ballProjection.y-robot->getRadius()*2;
			target.x = ballProjection.x-robot->getRadius()*1.5;
		} else {
			target.y = ballProjection.y+robot->getRadius()*2;
			target.x = ballProjection.x-robot->getRadius()*1.5;
		}
	}

	int halfGoal1 = rodetas::imageSize.y/2 + (rodetas::goalSize.y)*0.7;
	int halfGoal2 = rodetas::imageSize.y/2 - (rodetas::goalSize.y)*0.7;
	if(((ballProjection.y < halfGoal1 && ballProjection.y > halfGoal2 && ballProjection.x < imageSize.x*0.20))){
		target = Point(imageSize.x/2, imageSize.y/2);
	}

	// verifies the limits of the destination
	if (target.y < 0) target.y = 0;
	if (target.y > imageSize.y) target.y = imageSize.y;

	return target;
}