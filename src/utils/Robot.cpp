#include "Robot.h"

Robot::Robot(){
    initialize();
}

Robot::Robot(int _id){
    id = _id;

    initialize();
}

Robot::Robot(int _id, int _x, int _y){
    id = _id;
    position = Point(_x, _y);
    angle = 30;
    velocity = 0;
    command = Command(BACK_MOVE, 0, 0);
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

int Robot::y() const{
    return position.y;
}

int Robot::x() const{
    return position.x;
}

bool Robot::null(){
    return (position.x <= 0 && position.y <= 0);
}

float Robot::getVelocity(){
    return velocity;
}

void Robot::setRobotId(int _id){
    id = _id;
}

int Robot::getRobotId(){
    return id;
}

void Robot::setPosition(Point _pos){
    position = _pos;
    calculateSpeed();
}

Point Robot::getPosition() const {
    return position;
}

void Robot::setAngle(float _angle){
    angle = _angle;
}

float Robot::getAngle() const{
    return angle;
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

Point Robot::getTarget() const {
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

 float Robot::cosFrom(Robot _r) const{
    return cos((calcAngle(position, _r.getPosition()) - angle)/RADIAN_TO_DEGREE);
}

float Robot::cosFrom(Object _o) const{
    return cos((calcAngle(position, _o) - angle)/RADIAN_TO_DEGREE);  
}

float Robot::cosFrom(Point _p) const{
    return cos((calcAngle(position,_p) - angle)/RADIAN_TO_DEGREE);
}

float Robot::sinFrom(Robot _r) const{
    return sin((calcAngle(position,_r.getPosition()) - angle)/RADIAN_TO_DEGREE);
}

float Robot::sinFrom(Object _o) const{
    return sin((calcAngle(position, _o) - angle)/RADIAN_TO_DEGREE);
}

float Robot::sinFrom(Point _p) const{
    return sin((calcAngle(position,_p) - angle)/RADIAN_TO_DEGREE);
}

float Robot::distanceFrom(Robot _r) const{
    return distance(position, _r.getPosition());
}

float Robot::distanceFrom(Ball _b) const{
    return distance(position, _b.getPosition());
}

float Robot::distanceFrom(Object _o) const{
    return distance(position, _o);
}

float Robot::distanceFrom(Point _p) const{
    return distance(position, _p);
}