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

        transmission.send(strategy->getAttackCommand().first, strategy->getAttackCommand().second);
        transmission.send(strategy->getDefenseCommand().first, strategy->getDefenseCommand().second);
        transmission.send(strategy->getGoalCommand().first, strategy->getGoalCommand().second);
/* 
        for(int i=0 ; i<strategy->getNumStrategies() ; i++){
            transmission.send(strategy[i].getRobotId(), strategy[i].getCommand());
        } */
    }
            
    fps = timer.framesPerSecond();

    caller->notify("updateScreen");

    return true;
}

void GameModel::manualRobotControl(char direction, int pwm1, int pwm2){
    for(int i = 0 ; i < 3 ; i++) {
        transmission.send(i, Command(direction, pwm1, pwm2));
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

int GameModel::getFps(){
    return fps;
}

bool GameModel::getConnectionStatus(){
    return transmission.getConnectionStatus();
}

vector<rodetas::Object> GameModel::getPositions(){
    return vision.getPositions();
}
