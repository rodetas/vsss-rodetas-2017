#include "StrategyDefense.h"

StrategyDefense::StrategyDefense() : StrategyBase()	{

}

Command StrategyDefense::strategy(Robot* robot, Command command){
	Command c = command;

	c = stopStrategy(c);
	return c;
}

Point StrategyDefense::defineTarget(Robot* robot){

    Ball* ball = data->getBall();
    Point defenseTarget = Point(0,0);
	
	defenseTarget.x = data->getBall()->x() - (rodetas::imageSize.x * 0.2);

	// definir margem no meio do campo
	if (ball->y() > (rodetas::imageSize.y/2) ){
		defenseTarget.y = ball->y() - (rodetas::imageSize.y * 0.2); 
	} else {
		defenseTarget.y = ball->y() + (rodetas::imageSize.y * 0.2); 
	}

	//defenseTarget = applyPotencialField(defenseTarget, data->getRobot("attack").getPosition(), robot->getPosition());

	// verifies the destination limits
	// esse if tem que estar cobrindo isso tudo mesmo?
	if (defenseTarget.x < rodetas::goalSize.x) {
		defenseTarget.x = rodetas::goalSize.x;

		if(ball->y() < rodetas::imageSize.y/2)
			defenseTarget.y = rodetas::imageSize.y/2 - rodetas::goalArea.y/2;
		else 
			defenseTarget.y = rodetas::imageSize.y/2 + rodetas::goalArea.y/2;
	}	

	if ((defenseTarget.y > (imageSize.y/2-goalArea.y/2) || defenseTarget.y < (imageSize.y/2+goalArea.y/2)) && defenseTarget.x < goalArea.x) {
		defenseTarget.x = goalSize.x;
		if (ball->y() > imageSize.y/2){
			defenseTarget.y = imageSize.y/2+goalArea.y/2;
		}else{
			defenseTarget.y = imageSize.y/2-goalArea.y/2;
		}
	}

	if (robot->x() < (imageSize.x*0.2) && (robot->y() > (imageSize.y/2-goalArea.y/2) && robot->y() < (imageSize.y/2+goalArea.y/2))) { 
		defenseTarget.x = goalArea.x * 2;
		defenseTarget.y = ball->y();
	}

	return defenseTarget;
}