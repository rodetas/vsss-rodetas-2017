#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){
    Manipulation manipulation;
    manipulation.loadCalibration();  

	imageSize = manipulation.getImageSize();
	goalSize = manipulation.getGoal();
	goalArea = Point(imageSize.x*0.2, imageSize.y*0.6);

    targets.resize(3);

    /* team.resize(3);
    opponent.resize(3); */
}

void Strategy::initializeStrategies(){
    strategyAttack = new StrategyAttack();
    strategyDefense = new StrategyDefense();
    strategyGoal = new StrategyGoal();
}

Strategy* Strategy::getInstance(){

    if(instance == NULL){
        instance = new Strategy();
        instance->initializeStrategies();
    }

    return instance;
}

void Strategy::apply(vector<Object>& _team, vector<Object> _opponent, Object _ball){
    team.swap(_team);
    opponent.swap(_opponent); 
    ball = _ball;

    calculateBallProjection();
    // fazer if em relacao a quantidade do vetor team
    if (distance(team[GRAPHICPLAYER1], ball) < distance(team[GRAPHICPLAYER0], ball)){
        strategyAttack->apply(1);
        strategyDefense->apply(0);
    } else {
        strategyAttack->apply(0);
        strategyDefense->apply(1);
    }	

    strategyGoal->apply(2);
}

Object& Strategy::calculateBallProjection(){
	
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

int Strategy::getNumStrategies(){
    return StrategyFactory::getNumStrategies();
}

Object& Strategy::getBall(){
    return ball;
}

Object& Strategy::getBallProjection(){
    return ballProjection;
}

vector<Object>& Strategy::getTeam(){
    return team;
}

vector<Object>& Strategy::getOpponent(){
    return opponent;
}

Object& Strategy::getRobot(int id){
    return team[id];
}

Object& Strategy::getAttackRobot(){
    return strategyAttack->getRobot();
}

Object& Strategy::getDefenseRobot(){
    return strategyDefense->getRobot();
}

Object& Strategy::getGoalRobot(){
    return strategyGoal->getRobot();
}

Point& Strategy::getImageSize(){
    return imageSize;
}

Point& Strategy::getGoalSize(){
    return goalSize;
}

Point& Strategy::getGoalArea(){
    return goalArea;
}

void Strategy::setTargetOf(int id, Point target){
	targets[id] = target;
}

void Strategy::setPotencyFactor(float p){
    potency_factor = p;
}

void Strategy::setCurveFactor(float p){
    curve_factor = p;
}

pair<int, Command> Strategy::getAttackCommand(){
    return make_pair(strategyAttack->getRobotId(), strategyAttack->getCommand());
}

pair<int, Command> Strategy::getDefenseCommand(){
    return make_pair(strategyDefense->getRobotId(), strategyDefense->getCommand());
}

pair<int, Command> Strategy::getGoalCommand(){
    return make_pair(strategyGoal->getRobotId(), strategyGoal->getCommand());
}