#include "Strategy.h"

int Strategy::attackNumber = 0;
int Strategy::defenseNumber = 0;
int Strategy::goalNumber = 0;
int Strategy::nStrategies = 0;

bool Strategy::definedId = false;

Object Strategy::ballProjection;
vector<rodetas::Object> Strategy::lastBallPositions;
rodetas::Object Strategy::lastBallProjection;

Object Strategy::ball;

Point Strategy::imageSize = Point(0,0);
Point Strategy::goalSize = Point(0,0);
Point Strategy::goalArea = Point(0,0);

vector<rodetas::Object> Strategy::objects;
vector<rodetas::Object> Strategy::team;
vector<rodetas::Object> Strategy::opponent;
vector<Point> Strategy::targets;

Strategy::Strategy(){
    robotState = PARADO;
	nStrategies++;
    initialize();
}

Strategy::Strategy(int n){
    definedId = true;
	robotId = n;
}

void Strategy::initialize(){
	movimentation.setImage(imageSize);	
}

void Strategy::initStaticParameters(){
    Manipulation manipulation;
    manipulation.loadCalibration();  

	imageSize = manipulation.getImageSize();
	goalSize = manipulation.getGoal();
	goalArea = Point(imageSize.x*0.2, imageSize.y*0.6);

    targets.resize(3);
}

void Strategy::defineFunctions(){

	// COLOCAR O NUMERO DO ROBO NAS VARIAVEIS

	if(!definedId){

		if(team.size() == 1){
			
			attackNumber = 0;
			defenseNumber = 0;
			goalNumber = 0;

		} else if(team.size() == 2){

			defenseNumber = 0;
			goalNumber = 1;

		} else if(team.size() == 3){

			if (distance(team[GRAPHICPLAYER1], ball) < distance(team[GRAPHICPLAYER0], ball)){
				attackNumber = GRAPHICPLAYER1;
				defenseNumber = GRAPHICPLAYER0;
			} else {
				attackNumber = GRAPHICPLAYER0;
				defenseNumber = GRAPHICPLAYER1;
			}	

			goalNumber = GRAPHICPLAYER2;

		}
	}
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

void Strategy::calcBallProjection(){
	if(lastBallPositions.size() < 9){
		lastBallPositions.push_back(ball);
	} else {
		lastBallPositions.pop_back();
		lastBallPositions.insert(lastBallPositions.begin(), ball);
		ballProjection.x = ball.x + (lastBallPositions[0].x - lastBallPositions[8].x);
		ballProjection.y = ball.y + (lastBallPositions[0].y - lastBallPositions[8].y);

		if(ballProjection.x > imageSize.x || ballProjection.x < 0 || ballProjection.y > imageSize.y || ballProjection.y < 0){
			ballProjection = lastBallProjection;
		}

		lastBallProjection = ballProjection;
	}
}

void Strategy::setObjects(const vector<rodetas::Object>& t, const vector<rodetas::Object>& op, rodetas::Object b){
	team = t;
	opponent = op;
	ball = b;

	Strategy::calcBallProjection();
}

/*void Strategy::setObjects(const vector<rodetas::Object>& v){
    objects = v;
    ball = objects[GRAPHICBALL];
    Strategy::calcBallProjection();
}*/

Command Strategy::getCommand(){
    return movimentation.getMovement();
}   

int Strategy::getRobotId(){
	return robotId;
}