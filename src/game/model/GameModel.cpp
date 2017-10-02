#include "GameModel.h"

GameModel::GameModel(){
    play = false;
   	side = false;
    
    vision.initialize();
    strategy = Strategy::getInstance(); // init singleton
}

GameModel::~GameModel(){
    vision.cameraRelease();
    transmission.stopAllRobots(3);
}

bool GameModel::control(){

    vision.computerVision();

    strategy->apply(vision.getTeam(), vision.getOpponent(), vision.getBall());
   
    if(play){
        auto it = strategy->getRobotsBegin();
        auto itEnd = strategy->getRobotsEnd();

        for(;it != itEnd ; it++){
            transmission.send(it->getRobotId(), it->getCommand());
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

void GameModel::setSide(bool b){
    side = b;
}

void GameModel::setCaller(GameView* c){
    caller = c;
}

void GameModel::setPotencyFactor(float potency){
    strategy->setPotencyFactor(potency);
}

void GameModel::setCurveFactor(float curve){
    strategy->setCurveFactor(curve);
}

int GameModel::getFps(){
    return fps;
}

bool GameModel::getConnectionStatus(){
    return transmission.getConnectionStatus();
}

vector<Robot> GameModel::getTeam(){
    return vision.getTeam();
}

vector<Robot> GameModel::getOpponent(){
    return vision.getOpponent();
}

Ball GameModel::getBall(){
    return vision.getBall();
}