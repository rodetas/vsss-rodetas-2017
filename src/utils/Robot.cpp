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
}

void Robot::initialize(){
    velocity = 0;
    angle = 0;

    position = Point(0,0);
    target = Point(0,0);
    command = Command(0,0,STOPPED_MOVE);
    lastCommand = Command(0,0,STOPPED_MOVE);

    potencyFactor = 1.0f;
    curveFactor = 0.8f;
}

// IMPLEMENTAR TESTE
float Robot::calculateSpeed(){
   
	return distance((lastPositions.front()), (lastPositions[9]));
}

bool Robot::isBoard(){
   // cout<<getPosition()<<endl;
   // cout<<rodetas::imageSize<<endl;
	int halfGoal1 = rodetas::imageSize.y/2 + (rodetas::goalSize.y/2)*1.2;
	int halfGoal2 = rodetas::imageSize.y/2 - (rodetas::goalSize.y/2)*1.2;
	return (y() > (rodetas::imageSize.y*0.9) || y() < (rodetas::imageSize.y*0.10) || ((x() > (rodetas::imageSize.x*0.90) || x() < (rodetas::imageSize.x*0.10)) && (y() > halfGoal1 || y() < halfGoal2)));
}

bool Robot::isStopped(){
    float dis = distance((lastPositions.front()), (lastPositions[25]));
    //cout<<"dis: "<<dis<<endl;
    float vel = (getVelocity());
    //cout<<"vel: "<<vel<<endl;
    if(vel<5){
        return true;
    }else{
        return false;
    }
}

int Robot::y() const{
    return position.y;
}

int Robot::x() const{
    return position.x;
}

bool Robot::isNull(){
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

void Robot::setPotencyFactor(float _p){
    potencyFactor = _p;
}

float Robot::getPotencyFactor(){
    return potencyFactor;
}

void Robot::setCurveFactor(float _c){
    curveFactor = _c;
}

float Robot::getCurveFactor(){
    return curveFactor;
}

void Robot::setPosition(Point _pos){
    position = _pos;
    lastPositions.insert(lastPositions.begin(), position);
    if(lastPositions.size() >= 60){
        lastPositions.pop_back();
    }

    velocity = calculateSpeed();
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
    lastCommand = command;
    command = _command;
}

Command Robot::getCommand(){
    return command;
}

Command Robot::getLastCommand(){
    return lastCommand;
}

void Robot::setTarget(Point _target){
    target = _target;
}

Point Robot::getTarget() const {
    return target;
}

void Robot::setRadius(float r){
    radius = r;
}

float Robot::getRadius(){
    return radius;
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

float Robot::distanceFrom(Robot* _r) const{
    return distance(position, _r->getPosition());
}

float Robot::distanceFrom(Ball _b) const{
    return distance(position, _b.getPosition());
}

float Robot::distanceFrom(Ball* _b) const{
    return distance(position, _b->getPosition());
}

float Robot::distanceFrom(Object _o) const{
    return distance(position, _o);
}

float Robot::distanceFrom(Point _p) const{
    return distance(position, _p);
}