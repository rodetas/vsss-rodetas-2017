#include "StrategyAttack.h"

StrategyAttack::StrategyAttack() : StrategyBase(){

}

Command StrategyAttack::strategy(Robot* robot, Command command){

	Command c = command;
//	c = stopStrategy(c);
	c = cornerStrategy(c);
//	c = blockedStrategy(c);

	Ball* ball = data->getBall();

//	cout << robot

	if(robot->cosFrom(ball->getPosition()) > 0.9){

	}

	return c;
}

Point StrategyAttack::defineTarget(Robot* robot){
    Point target;// = data->getBall()->getPosition();
	Ball* ball = data->getBall();

	//target = ball->getBallProjection();
	target = ball->getPosition();

	Point centerGoal = Point(imageSize.x, imageSize.y/2);

	float angle = calcAngle(ball->getPosition(), centerGoal)/180.0;

	if(angle < 0){
		target.y -= 100.0*(1+(angle));
		target.x -= 50;
	} else {
		target.y += 100.0*(1-(angle));
		target.x -= 50;
	}


//	cout << robot->getId() << " " << robot->cosFrom(centerGoal) << " " << robot->sinFrom(centerGoal) <<  endl;

	float diff = robot->cosFrom(ball->getPosition()) - robot->cosFrom(centerGoal);
	cout << angle << endl;
	if(diff < 0.3 && diff > -0.3 && robot->distanceFrom(ball) < robot->getRadius()*3.0){

		target = centerGoal;
		cout << "ERROW" << endl;
	}

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
		
    
	// verifies the limits of the destination
	if (target.y < 0) target.y = 0;
	if (target.y > imageSize.y) target.y = imageSize.y;
*/

	return target;
}