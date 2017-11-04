#include "StrategyAttack.h"

StrategyAttack::StrategyAttack() : StrategyBase(){

}

Command StrategyAttack::strategy(Robot* robot, Command command){

	Command c = command;
//	c = stopStrategy(c);
	c = testStrategy(c);
	c = cornerStrategy(c);
// 	c = blockedStrategy(c);

	if (robot->isParallelGoal()){

		int halfGoal1 = rodetas::imageSize.y/2 + (rodetas::goalSize.y/2);
		int halfGoal2 = rodetas::imageSize.y/2 - (rodetas::goalSize.y/2);

		if ( robot->distanceFrom(data->getBall()->getPosition()) < robot->getRadius() * 1.2 && 
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

Command StrategyAttack::testStrategy(Command _command){
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

Point StrategyAttack::defineTarget(Robot* robot){
    Point target;// = data->getBall()->getPosition();
	Ball* ball = data->getBall();

	target = ball->getBallProjection();

	//target = ball->getPosition();



	Point centerGoal = Point(imageSize.x, imageSize.y/2);
	float angle_robot_goal = calcAngle(centerGoal, robot->getPosition());

	if(angle_robot_goal < 45.0 && angle_robot_goal > -45.0 && (robot->cosFrom(centerGoal) < -0.8 || robot->cosFrom(centerGoal) > 0.8) && 
		(robot->cosFrom(data->getBall()->getPosition()) < -0.8 || robot->cosFrom(data->getBall()->getPosition()) > 0.8) &&
		robot->x() < data->getBall()->x() && robot->distanceFrom(data->getBall()->getPosition()) < robot->getRadius()*2){

		target = centerGoal;
	} 


	

//	cout << calcAngle(centerGoal, robot->getPosition()) << endl; 

	/* float angle = calcAngle(ball->getPosition(), centerGoal)/180.0;

	if(angle < 0){
		target.y -= 150.0*(1+(angle));
		target.x -= robot->getRadius()*3.4;
	} else {
		target.y += 150.0*(1-(angle));
		target.x -= robot->getRadius()*3.4;
	} */

//	cout << robot->getId() << " " << robot->cosFrom(centerGoal) << " " << robot->sinFrom(centerGoal) <<  endl;

	

/*
	if(((cos_robot_ball < -0.8) || cos_robot_ball > 0.8) && distance(ball, robot) < imageSize.x * 0.08){
		target = { imageSize.x, imageSize.y/2};

	} else if(robot.x > (ball.x - (abs(robot.y-ball.y))) && robotState == DEFENDENDO) {

		if (ballProjection.x - 200 > goalSize.x){
			target.x = ballProjection.x - 200;
		}

		target = applyPotencialField(target, ballProjection, robot);

	} else {
		robotState = ATACANDO;
	}

	if(robot.x > ball.x) {
		robotState = DEFENDENDO;
	}
	// verifies if the robot is in the our goal area 
	if (target.y > (imageSize.y/2-goalArea.y/2) && target.y < (imageSize.y/2+goalArea.y/2) && target.x < goalArea.x) {
		target.x = imageSize.x* 0.2;
		target.y = ball.y;
	}
		
 */   

	//target = applyPotencialField(target, data->getRobot("defense").getPosition(), robot->getPosition());

	// verifies the limits of the destination
	if (target.y < 0) target.y = 0;
	if (target.y > imageSize.y) target.y = imageSize.y;

	return target;
}