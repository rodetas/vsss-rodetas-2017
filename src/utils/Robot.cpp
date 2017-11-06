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
    stoppedTime = 0;

    position = Point(0,0);
    target = Point(0,0);
    command = Command(0,0,STOPPED_MOVE);
    lastCommand = Command(0,0,STOPPED_MOVE);

    potencyFactor = 1.0f;
    curveFactor = 0.4f;
}

// IMPLEMENTAR TESTE
float Robot::calculateSpeed(){
    //cout << id << " " << lastPositions[9] << endl;
	return distance((lastPositions.front()), (lastPositions[9]));
}

bool Robot::isBoard(){
	return (y() > (rodetas::imageSize.y*0.9) || y() < (rodetas::imageSize.y*0.10) || ((x() > (rodetas::imageSize.x*0.85) || x() < (rodetas::imageSize.x*0.15))));
}

bool Robot::isParallelGoal(){
    return (cosFrom(Point(imageSize.x, y())) > -0.3 && cosFrom(Point(imageSize.x, y())) < 0.3);
}

bool Robot::calculateStopped(){
    if(velocity < 5.0){
        stoppedTime++;
        return true;
    }

    stoppedTime = 0;

    return false;
}

bool Robot::isStopped(){
    return stopped;
}

bool Robot::isStoppedFor(int time){
    if(stoppedTime >= time){
        return true;
    }

    return false;
}

bool Robot::isBlocked(){
    // se a distancia pro target e alta e o robo esta parado por muito tempo, entao é considerado travado
    if(distanceFrom(target) > radius*6 && isStoppedFor(90)){
        return true;
    } 

    return false;
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

int Robot::getId(){
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
    stopped = calculateStopped();
    projection = calculateRobotProjection();
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

Point Robot::getProjection(){
    return projection;
}

Point Robot::calculateRobotProjection(){
    Point aux = Point(position.x+radius/2, position.y+radius/2);
    
    projection.x = aux.x + (lastPositions[0].x - lastPositions[10].x);
    projection.y = aux.y + (lastPositions[0].y - lastPositions[10].y);

    if(projection.x > imageSize.x || projection.x < 0 || projection.y > imageSize.y || projection.y < 0){
        projection = lastRobotProjection;
    }
        
    lastRobotProjection = projection;

    return projection;
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