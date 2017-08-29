#ifndef M_GAME_H_
#define M_GAME_H_

#include "../Vision.h"
#include "../Strategies.h"
#include "../Transmission.h"
#include "../../utils/Timer.h"
#include "../../utils/Commons.h"
#include "../strategies/Strategy.h"


class GameView;
class GameModel {

public:
    GameModel();
	~GameModel();
    
    bool control();
    void manualRobotControl(char, int, int);
    
    void setCaller(GameView*);
    void setPlay(bool);
    void setSide(bool);
    
    int getFps();
    bool getConnectionStatus();
    
    Ball getBall();
	vector<Robot> getTeam();
	vector<Robot> getOpponent();

private:
    GameView* caller;
    Strategy* strategy;
    
    Timer timer;
    Vision vision;
    Transmission transmission;

    int fps;
    bool play;
    bool side;
    

};

#include "../view/GameView.h"

#endif