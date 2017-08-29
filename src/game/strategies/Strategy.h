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

    void apply(vector<Robot>& team, vector<Robot> opponent, Ball ball); // analisar a necessidade de retornar vector de command

     /**
     * Sets the potency factor value
     * \param f: the potency
     */
     void setPotencyFactor(float);
     
    /**
    * Sets the curve factor
    * \param f: the curve factor
    */
    void setCurveFactor(float);

    Ball& getBall();
    
    vector<Robot>& getTeam();

    vector<Robot>& getOpponent();

    Robot getRobot(string);

private:

    Strategy(); ///< private constructor for singleton
    
    void defineFunctionsForEachRobot();

    static Strategy* instance; ///< holds the class instance

    map<string, StrategyBase*> strategies;

    Ball ball; ///< contain the ball coordinates
    vector<Robot> team; ///< vector containing the team's positions
    vector<Robot> opponent;

    float curve_factor;
    float potency_factor;
};

#endif