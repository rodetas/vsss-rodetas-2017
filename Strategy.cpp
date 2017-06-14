#include "Strategy.h"

int Strategy::attackNumber = 0;
int Strategy::defenseNumber = 0;
int Strategy::goalNumber = 0;

Object Strategy::ballProjection;

Object Strategy::ball;

vector<rodetas::Object> Strategy::objects;
vector<Point> Strategy::targets;

Strategy::Strategy(){
    robotState = PARADO;
}

Strategy::Strategy(int n){
    nRobots = n;
}

void Strategy::initialize(){
    manipulation.loadCalibration();  
	imageSize = manipulation.getImageSize();
	goalSize = manipulation.getGoal();
	
//	movimentation.setImage(imageSize);	

	goalArea = {imageSize.x*0.2, imageSize.y*0.6};
}

void Strategy::defineFunctions(){

    if (distance(objects[GRAPHICPLAYER1], ball) < distance(objects[GRAPHICPLAYER0], ball)){
		attackNumber = GRAPHICPLAYER1;
		defenseNumber = GRAPHICPLAYER0;
	} else {
		attackNumber = GRAPHICPLAYER0;
		defenseNumber = GRAPHICPLAYER1;
	}	

	goalNumber = GRAPHICPLAYER2;
	ball = objects[GRAPHICBALL];

}

void Strategy::cornerStrategy(){
	
	/* movement along the corners */
	if (isBoard(robot)) {
		
		if (distance_robot_ball < 55){		

			if (robot.y > (imageSize.y/2)) {
				movimentation.turnLeft(120, 120);	
		    } else {
				movimentation.turnRight(120, 120);
			}
		}
	}	
}

// FICA CONFUSO QUANDO ESTA EXTAMENTE NO MEIO
Point Strategy::applyPotencialField(Point target, rodetas::Object toRepulsion, rodetas::Object toDestination){

	Point2i repulsion;
	Point2i factorRepulsion = { 5000, 20000 };

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

void Strategy::updateCalculus(){

    distance_robot_destination = distance(robot, destination);
    distance_ball_destination = distance(ball, destination);
    distance_robot_ball = distance(robot, ball);

	angle_robot_destination = calcAngle(destination, robot);
    sin_robot_destination = calcSen(destination, robot);
    cos_robot_destination = calcCos(destination, robot);
    cos_robot_ball = calcCos(ball, robot); 

    movimentation.updateCalculus(robot, ball, destination);

}

bool Strategy::isBoard(rodetas::Object object){
	int halfGoal1 = imageSize.y/2 + (goalSize.y/2)*1.2;
	int halfGoal2 = imageSize.y/2 - (goalSize.y/2)*1.2;
	return (object.y > (imageSize.y*0.9) || object.y < (imageSize.y*0.10) || ((object.x > (imageSize.x*0.90) || object.x < (imageSize.x*0.10)) && (object.y > halfGoal1 || object.y < halfGoal2)));
}

void Strategy::setVecTarget(int id, Point target){
	targets[id] = target;
}

void Strategy::setObjects(const vector<rodetas::Object>& v){
    objects = v;
}