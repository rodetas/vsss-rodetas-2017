#include "StrategyDefense.h"

StrategyDefense::StrategyDefense() : StrategyFactory()	{

}

void StrategyDefense::apply(int id){

    robot = data->getRobot(id);
	robot.id = id;

	movimentation.setRobot(robot);

	float velocity = movimentation.calculateSpeed();

	updateCalculus();

	destination = setTarget();

	Command movement(movimentation.movePlayers(destination));
}

Point StrategyDefense::setTarget(){
    Object ball = data->getBall();
    Point defenseTarget = Point(0,0);
	
	defenseTarget.x = data->getBall().x - (imageSize.x * 0.2);

	if (ball.y > (imageSize.y/2) ){
		defenseTarget.y = ball.y - (imageSize.y * 0.2); 
	} else {
		defenseTarget.y = ball.y + (imageSize.y * 0.2); 
	}

	defenseTarget = applyPotencialField(defenseTarget, data->getAttackRobot(), robot);

	// verifies the destination limits
	if (defenseTarget.x < goalSize.x) {
		defenseTarget.x = goalSize.x;

		if(ball.y < imageSize.y/2) defenseTarget.y = imageSize.y/2 - goalArea.y/2;
		else defenseTarget.y = imageSize.y/2 + goalArea.y/2;

	}	

	if ((defenseTarget.y > (imageSize.y/2-goalArea.y/2) || defenseTarget.y < (imageSize.y/2+goalArea.y/2)) && defenseTarget.x < goalArea.x) {
		defenseTarget.x = goalSize.x;
		if (ball.y > imageSize.y/2){
			defenseTarget.y = imageSize.y/2+goalArea.y/2;
		}else{
			defenseTarget.y = imageSize.y/2-goalArea.y/2;
		}
	}


	if (robot.x < (imageSize.x*0.2) && (robot.y > (imageSize.y/2-goalArea.y/2) && robot.y < (imageSize.y/2+goalArea.y/2))) { 
		defenseTarget.x = goalArea.x * 2;
		defenseTarget.y = ball.y;
	}

	data->setTargetOf(robot.id, defenseTarget);
	return defenseTarget;
}