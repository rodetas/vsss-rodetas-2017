#include "Strategy.h"

/*
 * Class constructor
 */
Strategy::Strategy(){
	movements.resize(3);
	objects.resize(7);
	targets.resize(3);

	state = PARADO;
}

/*	
 * Method to initialize strategy
 */
void Strategy::initialize(){
	manipulation.loadCalibration();  
	imageSize = manipulation.getImageSize();
	goalSize = manipulation.getGoal();
	
	movimentation.setImage(imageSize);	

	goalArea = {imageSize.x*0.2, imageSize.y*0.6};
}

/*
 * Defines and calls function accord to the strategy
 */
void Strategy::handleStrategies(){
	defineFunctions();
	calcBallProjection();	

	applyAttackStrategy();
	applyDefenseStrategy();
	applyGoalStrategy();



}

/*
 * Defines function of each robot
 */
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

	movimentation.setNumbers(attackNumber, defenseNumber, goalNumber);
}

/*
 * Control all things that attacker can do
 */
void Strategy::applyAttackStrategy(){

// commons functions to all strategies
///////////////////////////////////////////////////////////

	robot = objects[attackNumber]; // torna global o robo atual

	robot.id = attackNumber; // seta o id do robo atual

	movimentation.setRobot(robot); // seta na classe de movimentacao o robo atual

	float velocity = movimentation.calculateSpeed(); // calcula sua velocidade

	updateCalculus(robot, attackDestination); // update calculus (sin, cos, angle)

	attackDestination = setAttackTarget();

	Command movement(movimentation.movePlayers(attackDestination));

//////////////////////////////////////////////////////////	

	cornerStrategy("Attack");
}

Point Strategy::setAttackTarget(){

	Point target;

	target.x = ballProjection.x;
	target.y = ballProjection.y;

	/*target.x = ball.x;
	target.y = ball.y;*/

	if (isBoard(ball) && robot.x - 100 < ball.x){
		target.x = ball.x;
		target.y = ball.y;

	} else if(((cos_robot_ball < -0.8) || cos_robot_ball > 0.8) && distance(ball, robot) < imageSize.x * 0.08){
		target = {imageSize.x, imageSize.y/2};

	} else if(robot.x > (ball.x - (abs(robot.y-ball.y))) && state == DEFESA) {
		if (ballProjection.x - 200 > goalSize.x){
			target.x = ballProjection.x - 200;
		}
		target = applyPotencialField(target, ballProjection, robot);

	} else {
		state = ATAQUE;
	}

	if(robot.x > ball.x) {
		state = DEFESA;
	}

	// verifies if the robot is in the our goal area 
	if (target.y > (imageSize.y/2-goalArea.y/2) && target.y < (imageSize.y/2+goalArea.y/2) && target.x < goalArea.x) {
		target.x = imageSize.x* 0.2;
		target.y = ball.y;
	}
		
	// verifies the limits of the destination
	if (target.y < 0) target.y = 0;
	if (target.y > imageSize.y) target.y = imageSize.y;
	

	setVecTarget(robot.id, target);
	return target;
}


// FICA CONFUSO QUANDO ESTA EXTAMENTE NO MEIO

Point Strategy::applyPotencialField(Point target, rodetas::Object toRepulsion, rodetas::Object toDestination){

	Point2i repulsion;
	Point2i factorRepulsion = {5000, 20000};

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

void Strategy::applyDefenseStrategy(){

// commons functions to all strategies
//////////////////////////////////////////////////////////

	robot = objects[defenseNumber];

	robot.id = defenseNumber;

	movimentation.setRobot(robot);

	float velocity = movimentation.calculateSpeed();

	updateCalculus(robot, defenseDestination);

	defenseDestination = setDefenseTarget();

	Command movement(movimentation.movePlayers(defenseDestination));


//////////////////////////////////////////////////////////

	///////////////////////////////////
	//////// @TODO ESTRATEGIAS ////////
	///////////////////////////////////

}

Point Strategy::setDefenseTarget(){
	Point defenseTarget = Point(0,0);
	
	defenseTarget.x = ball.x - (imageSize.x * 0.2);

	if (ball.y > (imageSize.y/2) ){
		defenseTarget.y = ball.y - (imageSize.y * 0.2); 
	} else {
		defenseTarget.y = ball.y + (imageSize.y * 0.2); 
	}

	defenseTarget = applyPotencialField(defenseTarget, objects[attackNumber], robot);

	// verifies the limits of the destination
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

	setVecTarget(robot.id, defenseTarget);
	return defenseTarget;
}

void Strategy::applyGoalStrategy(){

	// commons functions to all strategies
//////////////////////////////////////////////////////////

	robot = objects[goalNumber];
	robot.id = goalNumber;

	movimentation.setRobot(robot);

	float velocity = movimentation.calculateSpeed();

	updateCalculus(robot, goalDestination);

	goalDestination = setGoalTarget();


	Command movement(movimentation.movePlayers(goalDestination));


	if (distance_robot_ball<60 && robot.x < ball.x) {
		if (robot.y<ball.y) {
			movimentation.turnLeft(120, 120);
		} else {
			movimentation.turnRight(120, 120);
		}
	}

	if (distance_robot_destination < 35 && distance_robot_ball > 300) {
	
		float cosRobot = cos(robot.angle/RADIAN_TO_DEGREE);
		
		if (cosRobot > 0.2) {
			movimentation.turnRight(5 + 40*cosRobot, 5 + 40*cosRobot);
		} else if (cosRobot < -0.2 ) {
			movimentation.turnLeft(5 + 40*-cosRobot, 5 + 40*-cosRobot);
		}
	}

//////////////////////////////////////////////////////////


	///////////////////////////////////
	//////// @TODO ESTRATEGIAS ////////
	///////////////////////////////////

}

Point Strategy::setGoalTarget(){
	Point goalTarget = Point(0,0);

	goalTarget.x = imageSize.x*0.13;
	goalTarget.y = imageSize.y/2 - (imageSize.y/2-ballProjection.y)/2;
	
	if (robot.x<goalSize.x*1.2 && distance_robot_ball>300) {
		goalTarget.x = goalSize.x+250;
		goalTarget.y = imageSize.y/2;
	} else if (goalTarget.y > (imageSize.y/2-goalArea.y/2) && goalTarget.y < (imageSize.y/2+goalArea.y/2) && ballProjection.x < imageSize.x*0.25 && robot.x < ball.x) {
		goalTarget.x = ballProjection.x; //ou manda direto na bola?
		goalTarget.y = ballProjection.y; //ou manda direto na bola?
	} else if (goalTarget.y < (imageSize.y/2-goalArea.y/2)) {
		goalTarget.y = (imageSize.y/2-goalArea.y/2);
	} else if (goalTarget.y > (imageSize.y/2+goalArea.y/2)) {
		goalTarget.y = (imageSize.y/2+goalArea.y/2);
	}

	setVecTarget(robot.id, goalTarget);
	return goalTarget;
}

bool Strategy::isBoard(rodetas::Object object){
	int halfGoal1 = imageSize.y/2 + (goalSize.y/2)*1.2;
	int halfGoal2 = imageSize.y/2 - (goalSize.y/2)*1.2;
	return (object.y > (imageSize.y*0.9) || object.y < (imageSize.y*0.10) || ((object.x > (imageSize.x*0.90) || object.x < (imageSize.x*0.10)) && (object.y > halfGoal1 || object.y < halfGoal2)));
}


void Strategy::cornerStrategy(string type){
	
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

/*
 * Updates the calculus about robot position
 */
void Strategy::updateCalculus(rodetas::Object robot, Point destination){

	distance_robot_destination = distance(robot, destination);
    distance_ball_destination = distance(ball, destination);
    distance_robot_ball = distance(robot, ball);

	angle_robot_destination = calcAngle(destination, robot);
    sinAngle_robot_destination = calcSen(destination, robot);
    cosAngle_robot_destination = calcCos(destination, robot);
    cos_robot_ball = calcCos(ball, robot); 

    movimentation.updateCalculus(robot, destination);

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

void Strategy::setObjects(vector<rodetas::Object> o){
	this->objects = o;
}

void Strategy::setPowerPotency(float a){
	movimentation.setPower(a);
}

void Strategy::setPowerCurve(float a){
	movimentation.setPowerCurve(a);
}

void Strategy::setVecTarget(int id, Point target){
	targets[id] = target;
}

vector<Command> Strategy::getMovements(){
	//retorna movimentos sem alteracao
	movements = movimentation.getMovements();
	return movements;
}

vector<Point> Strategy::getTargets(){
	for(int i=0 ; i<targets.size() ; i++){
		//cout << targets[i] << endl;
	}
	return targets;
}