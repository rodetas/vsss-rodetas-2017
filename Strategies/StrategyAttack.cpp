#include "StrategyAttack.h"

StrategyAttack::StrategyAttack() : Strategy(){

}

void StrategyAttack::apply(){
    robot = team[attackNumber];
    robot.id = attackNumber;

    movimentation.setRobot(robot); // seta na classe de movimentacao o robo atual

    float velocity = movimentation.calculateSpeed(); // calcula sua velocidade
    
	updateCalculus(); // update calculus (sin, cos, angle)
	destination = setTarget();

	Command movement(movimentation.movePlayers(destination));

//	cornerStrategy();
}

Point StrategyAttack::setTarget(){
    Point target;

/*	target.x = ballProjection.x;
	target.y = ballProjection.y;
*/
	target.x = ball.x;
	target.y = ball.y;
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

	setVecTarget(robot.id, target);

	return target;
}