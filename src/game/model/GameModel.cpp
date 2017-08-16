#include "GameModel.h"

GameModel::GameModel(){
    play = false;
   	side = false;

    vision.initialize();
    
    StrategyFactory::initStaticParameters(); // precisa ser primeiro

    strategies.push_back(new StrategyAttack());
//	strategies.push_back(new StrategyDefense());
//	strategies.push_back(new StrategyGoal());/

    for(auto s: strategies){
        s->initialize();
    }
}

GameModel::~GameModel(){
    vision.cameraRelease();
    transmission.stopAllRobots(3);

    for(auto s: strategies){
        delete s;
    }
}

bool GameModel::control(){
    vision.computerVision();

    StrategyFactory::setObjects(vision.getTeam(), vision.getOpponent(), vision.getBall());
    StrategyFactory::defineFunctions();

    for(int i=0 ; i<strategies.size() ; i++){
        strategies[i]->apply();
    }

    if(play){
        for(int i=0 ; i<strategies.size() ; i++){
            transmission.send(strategies[i]->getRobotId(), strategies[i]->getCommand());
        }
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
