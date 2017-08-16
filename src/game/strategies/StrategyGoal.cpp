#include "StrategyGoal.h"

StrategyGoal::StrategyGoal() : StrategyFactory() {

}

void StrategyGoal::apply(){

	robot = objects[goalNumber];
	robot.id = goalNumber;

	movimentation.setRobot(robot);

	float velocity = movimentation.calculateSpeed();

	updateCalculus();

	destination = setTarget();

	Command movement(movimentation.movePlayers(destination));
}

Point StrategyGoal::setTarget(){

    Point goalTarget = Point(0,0);

	goalTarget.x = imageSize.x*0.10;
	goalTarget.y = imageSize.y/2 - (imageSize.y/2-ballProjection.y)/2;
	
	if (robot.x<goalSize.x*1.2 && distance_robot_ball>300) {
		goalTarget.x = goalSize.x+250;
		goalTarget.y = imageSize.y/2;
	} else if (goalTarget.y > (imageSize.y/2-goalArea.y/2) && goalTarget.y < (imageSize.y/2+goalArea.y/2) && ballProjection.x < imageSize.x*0.25 && robot.x < ball.x) {
		goalTarget.x = ballProjection.x; //ou manda direto na bola?
		goalTarget.y = ballProjection.y; //ou manda direto na bola?
	} else if (goalTarget.y < (imageSize.y/2-goalArea.y/2)) {
		goalTarget.y = (imageSize.y/2-goalArea.y/2);
	} else if (goalTarget.y > (imageSize.y/2+goalArea.y/2)) {
		goalTarget.y = (imageSize.y/2+goalArea.y/2);
	}

	setVecTarget(robot.id, goalTarget);
	return goalTarget;
}