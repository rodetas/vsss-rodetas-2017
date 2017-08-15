#include "StrategyFactory.h"

int StrategyFactory::attackNumber = 0;
int StrategyFactory::defenseNumber = 0;
int StrategyFactory::goalNumber = 0;
int StrategyFactory::nStrategies = 0;

float StrategyFactory::curve_factor;
float StrategyFactory::potency_factor;

bool StrategyFactory::definedId = false;

Object StrategyFactory::ballProjection;
vector<rodetas::Object> StrategyFactory::lastBallPositions;
rodetas::Object StrategyFactory::lastBallProjection;

Object StrategyFactory::ball;

Point StrategyFactory::imageSize = Point(0,0);
Point StrategyFactory::goalSize = Point(0,0);
Point StrategyFactory::goalArea = Point(0,0);

vector<rodetas::Object> StrategyFactory::objects;
vector<rodetas::Object> StrategyFactory::team;
vector<rodetas::Object> StrategyFactory::opponent;
vector<Point> StrategyFactory::targets;

StrategyFactory::StrategyFactory(){
    robotState = PARADO;
	nStrategies++;
    initialize();
}

StrategyFactory::StrategyFactory(int n){
    definedId = true;
	robotId = n;
}

void StrategyFactory::initialize(){
	movimentation.setImage(imageSize);	
}

void StrategyFactory::initStaticParameters(){
    Manipulation manipulation;
    manipulation.loadCalibration();  

	imageSize = manipulation.getImageSize();
	goalSize = manipulation.getGoal();
	goalArea = Point(imageSize.x*0.2, imageSize.y*0.6);

    targets.resize(3);
}

void StrategyFactory::defineFunctions(){

	// COLOCAR O NUMERO DO ROBO NAS VARIAVEIS

	if(!definedId){

		if(nStrategies == 1){
			
			attackNumber = 0;
			defenseNumber = 0;
			goalNumber = 0;

		} else if(nStrategies == 2){

			defenseNumber = 0;
			goalNumber = 1;

		} else if(nStrategies == 3){

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
	attackNumber = 1;
}

void StrategyFactory::cornerStrategy(){
	
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

void StrategyFactory::setVecTarget(int id, Point target){
	targets[id] = target;
}

Object StrategyFactory::calculateBallProjection(){
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

	return ballProjection;
}

void StrategyFactory::setObjects(const vector<rodetas::Object>& t, const vector<rodetas::Object>& op, rodetas::Object b){
	team = t;
	opponent = op;
	ball = b;

	StrategyFactory::calculateBallProjection();
}

Command StrategyFactory::getCommand(){
    return movimentation.getMovement();
}   

int StrategyFactory::getRobotId(){
	return robot.id;
}

void StrategyFactory::setPotencyFactor(float f){
	StrategyFactory::potency_factor = f;
}

void StrategyFactory::setCurveFactor(float f){
	StrategyFactory::curve_factor = f;
}