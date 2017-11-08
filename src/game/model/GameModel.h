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
    void setTargetFromScreen(bool);
    void setTargetOf(Point, int);
    void changeGameSide();
    void changePositionClick();

    void setPotencyFactor(float, int);
    void setCurveFactor(float, int);

    Robot* getRobot(int);

    int getFps();
    bool getConnectionStatus();

    vector<Point> getTargets();
    
    Ball getBall();
	vector<Robot> getTeam();
    vector<Robot> getOpponent();

private:
    GameView* caller;
    Strategy* strategy;
    
    Timer timer;
    Vision vision;
    Transmission transmission;

    vector<Robot*> robots;
    vector<Robot*> opponent;
    Ball* ball;

    int fps;
    bool play;
    bool targetFromScreen;
    bool enableChangePosition;
    

};

#include "../view/GameView.h"

#endif