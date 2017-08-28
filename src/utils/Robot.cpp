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

vector<Point> Robot::getLastPositions(){
    return lastPositions;
}

/* float Robot::getCosFrom(Robot _r){
    return calcCos(_r.getPosition(), position);
}

float Robot::getCosFrom(Object _o){
    
}

float Robot::getCosFrom(Point _p){

}
 
float Robot::getSinFrom(Robot _r){
    return calcSen(_r.getPosition(), position);
}

float Robot::getSinFrom(Object _o){
    return calcSen(_o, position);
}

float Robot::getSinFrom(Point _p){
    return calcSen(_p, position);
}
*/
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