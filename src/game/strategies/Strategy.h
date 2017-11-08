#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "../../utils/Manipulation.h"
#include "../../utils/Commons.h"
#include "../../utils/Robot.h"
#include "../../utils/Ball.h"
#include "../Strategies.h"

#include <map>

using namespace rodetas;

class Strategy{

public:
    // Singleton Implementation
    static Strategy* getInstance();

    void initializeStrategies();

    void initializeRobots(vector<Robot*>);

    void apply(vector<Robot*> team, vector<Robot*> opponent, Ball* ball, bool targetFromScreen = false, bool enableChangePosition = true);

    Ball* getBall();

    Robot getRobot(string);

private:

    Strategy(); ///< private constructor for singleton
    
    void defineFunctionsForEachRobot(vector<Robot*>);

    static Strategy* instance; ///< holds the class instance

    map<string, StrategyBase*> strategies;

    Ball* ball; ///< contain the ball coordinates
    map<string, Robot*> team; ///< vector containing the team's positions
    vector<Robot*> opponent;

    int timeLastChange;

    float curve_factor;
    float potency_factor;
};

#endif