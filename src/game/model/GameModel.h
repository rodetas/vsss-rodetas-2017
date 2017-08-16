#ifndef M_GAME_H_
#define M_GAME_H_

#include "../Vision.h"
#include "../Strategies.h"
#include "../Transmission.h"
#include "../../utils/Timer.h"
#include "../../utils/Commons.h"

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
    vector<rodetas::Object> getPositions();

private:
    GameView* caller;
    vector<StrategyFactory*> strategies;

    Timer timer;
    Vision vision;
    Transmission transmission;

    int fps;
    bool play;
    bool side;
    

};

#include "../view/GameView.h"

#endif