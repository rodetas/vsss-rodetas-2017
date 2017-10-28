#include "StrategyDefense.h"

StrategyDefense::StrategyDefense() : StrategyBase()	{

}

Command StrategyDefense::strategy(Robot* robot, Command command){
	Command c = command;

	c = stopStrategy(c);

	return c;
}

Point StrategyDefense::defineTarget(Robot* robot){

	Point defenseTarget = Point(700, (imageSize.y/2)+200);


	return defenseTarget;
}