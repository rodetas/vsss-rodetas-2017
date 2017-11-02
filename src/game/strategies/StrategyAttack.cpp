#include "StrategyAttack.h"

StrategyAttack::StrategyAttack() : StrategyBase(){

}

Command StrategyAttack::strategy(Robot* robot, Command command){

	Command c = command;
//	c = stopStrategy(c);
	c = cornerStrategy(c);
// c = blockedStrategy(c);

	return c;
}

Point StrategyAttack::defineTarget(Robot* robot){
    Point target;// = data->getBall()->getPosition();
	Ball* ball = data->getBall();

	target = ball->getBallProjection();

	Point centerGoal = Point(imageSize.x, imageSize.y/2);

	//cout << robot->getId() << " " << robot->cosFrom(centerGoal) << " " << robot->sinFrom(centerGoal) << endl;

	// robo esta atras da bola, nao pode acerta-la pois podera fazer gol contra
	/* if(robot->x() > ball->x()){
		if(ball->getBallProjection().y > imageSize.y/2){
			target.y -= robot->getRadius()*3;
		} else {
			target.y += robot->getRadius()*3;
		}
	} */

	if( (robot->cosFrom(ball->getPosition()) < -0.9 || robot->cosFrom(ball->getPosition()) > 0.9) && robot->distanceFrom(ball) < robot->getRadius()*3.5 &&
		(robot->cosFrom(centerGoal) < -0.8 || robot->cosFrom(centerGoal) > 0.8)){

		target = Point(imageSize.x, imageSize.y/2);
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