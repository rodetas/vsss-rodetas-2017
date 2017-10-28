#include "StrategyAttack.h"

StrategyAttack::StrategyAttack() : StrategyBase(){

}

Command StrategyAttack::strategy(Robot* robot, Command command){

	Command c = command;
//	c = stopStrategy(c);
	return c;
}

Point StrategyAttack::defineTarget(Robot* robot){
    Point target;
	
/*	target.x = ballProjection.x;
	target.y = ballProjection.y;
*/
 	target.x = data->getBall()->x();
	target.y = data->getBall()->y();

	float m = (target.y-robot->y())/(target.x-robot->x());

	Point parcial;

	if(target.x > robot->x()){
		parcial.x = robot->x()+70;
		parcial.y = m*(parcial.x-robot->x()) + robot->y();
	} else {
		parcial.x = robot->x()-70;
		parcial.y = m*(parcial.x-robot->x()) + robot->y();
	}

//	cout << parcial << " " << m << endl;

	target = parcial;

//	target = data->getBall()->getBallProjection();

	/* target.x = imageSize.x/2;
	target.y = imageSize.y/2; */
/*
	if (isBoard(ball) && robot.x - 100 < ball.x){
		target.x = ball.x;
		target.y = ball.y;

	} else if(((cos_robot_ball < -0.8) || cos_robot_ball > 0.8) && distance(ball, robot) < imageSize.x * 0.08){
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

//	data->setTargetOf(robot.id, target);

	return target;
}