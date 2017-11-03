#include "StrategyDefense.h"

StrategyDefense::StrategyDefense() : StrategyBase()	{

}

Command StrategyDefense::strategy(Robot* robot, Command command){
	Command c = command;

	c = stopStrategy(c);

	return c;
}

Point StrategyDefense::defineTarget(Robot* robot){

	Point defenseTarget = Point(imageSize.x/4, imageSize.y/2);


	return defenseTarget;
}