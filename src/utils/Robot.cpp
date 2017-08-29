#include "Robot.h"

Robot::Robot(){
    initialize();
}

Robot::Robot(int _id){
    id = _id;

    initialize();
}

void Robot::initialize(){
    velocity = 0;
    angle = 0;

    position = Point(0,0);
    target = Point(0,0);
}

// IMPLEMENTAR TESTE
float Robot::calculateSpeed(){

    lastPositions.insert(lastPositions.begin(), position);

    if(lastPositions.size() >= velocityPrecision){
        lastPositions.pop_back();
    }

	return distance(*(lastPositions.begin()), *(lastPositions.end()));
}

bool Robot::isBoard(){
	int halfGoal1 = rodetas::imageSize.y/2 + (rodetas::goalSize.y/2)*1.2;
	int halfGoal2 = rodetas::imageSize.y/2 - (rodetas::goalSize.y/2)*1.2;
	return (y() > (rodetas::imageSize.y*0.9) || y() < (rodetas::imageSize.y*0.10) || ((x() > (rodetas::imageSize.x*0.90) || x() < (rodetas::imageSize.x*0.10)) && (y() > halfGoal1 || y() < halfGoal2)));
}

int Robot::y(){
    return position.y;
}

int Robot::x(){
    return position.x;
}

float Robot::getVelocity(){
    return velocity;
}

void Robot::setId(int _id){
    id = _id;
}

int Robot::getId(){
    return id;
}

void Robot::setPosition(Point _pos){
    position = _pos;
    calculateSpeed();
}

Point Robot::getPosition(){
    return position;
}

void Robot::setCommand(Command _command){
    command = _command;
}

Command Robot::getCommand(){
    return command;
}

void Robot::setTarget(Point _target){
    target = _target;
}

Point Robot::getTarget(){
    return target;
}

void Robot::setLastPositions(vector<Point> _vec){
    lastPositions = _vec;
}

vector<Point>::iterator Robot::getLastPositionsBegin(){
    return lastPositions.begin();
}

vector<Point>::iterator Robot::getLastPositionsEnd(){
    return lastPositions.end();
}

 float Robot::cosFrom(Robot _r){
    return cos((calcAngle(position, _r.getPosition()) - angle)/RADIAN_TO_DEGREE);
}

float Robot::cosFrom(Object _o){
    return cos((calcAngle(position, _o) - angle)/RADIAN_TO_DEGREE);  
}

float Robot::cosFrom(Point _p){
    return cos((calcAngle(position,_p) - angle)/RADIAN_TO_DEGREE);
}

float Robot::sinFrom(Robot _r){
    return sin((calcAngle(position,_r.getPosition()) - angle)/RADIAN_TO_DEGREE);
}

float Robot::sinFrom(Object _o){
    return sin((calcAngle(position, _o) - angle)/RADIAN_TO_DEGREE);
}

float Robot::sinFrom(Point _p){
    return sin((calcAngle(position,_p) - angle)/RADIAN_TO_DEGREE);
}

float Robot::distanceFrom(Robot _r){
    return distance(position, _r.getPosition());
}

float Robot::distanceFrom(Ball _b){
    return distance(position, _b.getPosition());
}

float Robot::distanceFrom(Object _o){
    return distance(position, _o);
}

float Robot::distanceFrom(Point _p){
    return distance(position, _p);
}