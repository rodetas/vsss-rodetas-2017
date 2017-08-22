#include "StrategyFactory.h"

int StrategyFactory::nStrategies = 0;

bool StrategyFactory::definedId = false;

StrategyFactory::StrategyFactory(){
    robotState = PARADO;
	nStrategies++;

	data = Strategy::getInstance();
	imageSize = data->getImageSize();
    goalSize = data->getGoalSize();
    goalArea = data->getGoalArea();
	movimentation.setImage(imageSize);	

//    initialize();
}

StrategyFactory::StrategyFactory(int n){
    definedId = true;
	robotId = n;
}

void StrategyFactory::initialize(){
	// ANALISAR A NECESSIDADE DE COLOCAR STRATEGY NO MOVIMENTATION
	
}

void StrategyFactory::cornerStrategy(){
	
	/* movement along the corners */
	if (isBoard(robot)){
		
		if (distance_robot_ball < 55){		

			if (robot.y > (data->getImageSize().y/2)){
				movimentation.turnLeft(120, 120);	
		    } else {
				movimentation.turnRight(120, 120);
			}
		}
	}	
}

// FICA CONFUSO QUANDO ESTA EXTAMENTE NO MEIO
Point StrategyFactory::applyPotencialField(Point target, rodetas::Object toRepulsion, rodetas::Object toDestination){

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

	target.x += (repulsion.x);
	target.y += (repulsion.y);

	return target;
}

void StrategyFactory::updateCalculus(){

	Object ball = data->getBall();

    distance_robot_destination = distance(robot, destination);
    distance_ball_destination = distance(ball, destination);
    distance_robot_ball = distance(robot, ball);

	angle_robot_destination = calcAngle(destination, robot);
    sin_robot_destination = calcSen(destination, robot);
    cos_robot_destination = calcCos(destination, robot);
    cos_robot_ball = calcCos(ball, robot); 

    movimentation.updateCalculus(robot, ball, destination);

}

bool StrategyFactory::isBoard(rodetas::Object object){
	int halfGoal1 = imageSize.y/2 + (goalSize.y/2)*1.2;
	int halfGoal2 = imageSize.y/2 - (goalSize.y/2)*1.2;
	return (object.y > (imageSize.y*0.9) || object.y < (imageSize.y*0.10) || ((object.x > (imageSize.x*0.90) || object.x < (imageSize.x*0.10)) && (object.y > halfGoal1 || object.y < halfGoal2)));
}

Command StrategyFactory::getCommand(){
    return movimentation.getMovement();
}   

int StrategyFactory::getRobotId(){
	return robot.id;
}

Object& StrategyFactory::getRobot(){
	return robot;
}

int StrategyFactory::getNumStrategies(){
	return nStrategies;
}