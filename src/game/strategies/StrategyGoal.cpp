#include "StrategyGoal.h"

StrategyGoal::StrategyGoal() : StrategyBase() {

}

Command StrategyGoal::strategy(Robot& robot, Command command){

	return command;
}

Point StrategyGoal::defineTarget(const Robot& robot){

    Point goalTarget = Point(0,0);
	Point ballProjection = data->getBall().getBallProjection();

	goalTarget.x = rodetas::imageSize.x*0.10;
	goalTarget.y = rodetas::imageSize.y/2 - (rodetas::imageSize.y/2-ballProjection.y)/2;
	
	if (robot.x() < goalSize.x*1.2 && robot.distanceFrom(data->getBall()) > 300) {
		goalTarget.x = goalSize.x+250;
		goalTarget.y = imageSize.y/2;
	} else if (goalTarget.y > (imageSize.y/2-goalArea.y/2) && goalTarget.y < (imageSize.y/2+goalArea.y/2) && ballProjection.x < imageSize.x*0.25 && robot.x() < data->getBall().x()) {
		goalTarget.x = ballProjection.x; //ou manda direto na bola?
		goalTarget.y = ballProjection.y; //ou manda direto na bola?
	} else if (goalTarget.y < (imageSize.y/2-goalArea.y/2)) {
		goalTarget.y = (imageSize.y/2-goalArea.y/2);
	} else if (goalTarget.y > (imageSize.y/2+goalArea.y/2)) {
		goalTarget.y = (imageSize.y/2+goalArea.y/2);
	}

	return goalTarget;
}