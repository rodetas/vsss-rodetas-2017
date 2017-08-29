#include "StrategyBase.h"

int StrategyBase::nStrategies = 0;

StrategyBase::StrategyBase(){
    robotState = PARADO;
	nStrategies++;

	data = Strategy::getInstance();
}

void StrategyBase::apply(){
    Point target = defineTarget();
    robot.setCommand(movimentation.movePlayers(robot));
    
}

void StrategyBase::cornerStrategy(){
	
	/* movement along the corners */
	if (robot.isBoard()){
		
		if (robot.distanceFrom(data->getBall()) < 55){		

			if (robot.y() > (rodetas::imageSize.y/2)){
				movimentation.turnLeft(120, 120);	
		    } else {
				movimentation.turnRight(120, 120);
			}
		}
	}	
}

// FICA CONFUSO QUANDO ESTA EXTAMENTE NO MEIO
Point StrategyBase::applyPotencialField(Point target, Point toRepulsion, Point toDestination){
    
    Point2i repulsion;
    Point2i factorRepulsion = Point2i(5000,20000);//{ 5000, 20000 };

    float sin_repulsion_destination = sin((calcAngle(toDestination, toRepulsion))/RADIAN_TO_DEGREE);
    float cos_repulsion_destination = cos((calcAngle(toDestination, toRepulsion))/RADIAN_TO_DEGREE);
    float distance_repulsion_destination = distance(toRepulsion, toDestination);

    if(sin_repulsion_destination > 0){
        sin_repulsion_destination = 1 - sin_repulsion_destination;
    } else {
        sin_repulsion_destination = abs(sin_repulsion_destination) - 1;
    }

    repulsion.x = (cos_repulsion_destination / (distance_repulsion_destination * 0.6)) * factorRepulsion.x;
    repulsion.y = (sin_repulsion_destination / (distance_repulsion_destination * 0.6)) * factorRepulsion.y;

    robot.setTarget(Point(repulsion.x, repulsion.y));

    return robot.getTarget();
}

void StrategyBase::setRobot(Robot _robot){
    robot = _robot;
}

int StrategyBase::getNumStrategies(){
	return nStrategies;
}