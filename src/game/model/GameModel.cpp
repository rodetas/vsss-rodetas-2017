#include "GameModel.h"

GameModel::GameModel(){
    play = false;
    targetFromScreen = false;
    
    for(int i=0 ; i<3 ; i++){
        robots.push_back(new Robot(i));
        opponent.push_back(new Robot(i));
    }

    ball = new Ball();

    vision.initialize(robots, opponent,ball);
    strategy = Strategy::getInstance(); // init singleton
    strategy->initializeRobots(robots);
}

GameModel::~GameModel(){

    vision.cameraRelease();
    transmission.stopAllRobots(3);

    for(int i=0 ; i<3 ; i++){
        delete robots[i];
        delete opponent[i];
    }
}

bool GameModel::control(){

    vision.computerVision();

    strategy->apply(robots, opponent, ball, targetFromScreen);
   
    if(play){
        for(int i=0 ; i<robots.size() ; i++){
            transmission.send(robots[i]->getId(), robots[i]->getCommand());
        }
    }

    fps = timer.framesPerSecond();

    caller->updateScreen();

    return true;
}

void GameModel::manualRobotControl(char direction, int pwm1, int pwm2){
    for(int i = 0 ; i < 3 ; i++) {
        transmission.send(i, Command(pwm1, pwm2, direction));
    }
}

void GameModel::setPlay(bool b){
    play = b;
}

void GameModel::setTargetFromScreen(bool t){
    targetFromScreen = t;
}

void GameModel::changeGameSide(){
    vision.changeGameSide();
}

void GameModel::setCaller(GameView* c){
    caller = c;
}

void GameModel::setPotencyFactor(float potency, int id){
    robots[id]->setPotencyFactor(potency);
}

void GameModel::setCurveFactor(float curve, int id){
    robots[id]->setCurveFactor(curve);
}

void GameModel::setTargetOf(Point target, int id){
    robots[id]->setTarget(target);
}

vector<Point> GameModel::getTargets(){
    vector<Point> targets;
    for(int i=0 ; i<robots.size() ; i++){
        targets.push_back(robots[i]->getTarget());
    }

    return targets;
}

int GameModel::getFps(){
    return fps;
}

bool GameModel::getConnectionStatus(){
    return transmission.getConnectionStatus();
}

Robot* GameModel::getRobot(int id){
    return robots[id];
}

vector<Robot> GameModel::getTeam(){

    vector<Robot> _robots;
    for(int i=0 ; i<robots.size() ; i++){
        // desreferenciando a variavel
        _robots.push_back(*(robots[i]));
    }

    return _robots;
}

vector<Robot> GameModel::getOpponent(){

    vector<Robot> _robots;
    for(int i=0 ; i<opponent.size() ; i++){
        // desreferenciando a variavel
        _robots.push_back(*(opponent[i]));
    }

    return _robots;
}

Ball GameModel::getBall(){
    return *ball;
}