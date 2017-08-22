#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "../../utils/Commons.h"
#include "../Strategies.h"

using namespace rodetas;

class Strategy{

public:

    // Singleton Implementation
    static Strategy* getInstance();

    void initializeStrategies();

    void apply(vector<Object>& team, vector<Object> opponent, Object ball); // analisar a necessidade de retornar vector de command

    /**
     * Calculates the ball projection
     * \return the ball projection
     */
    Object& calculateBallProjection();

    int getNumStrategies();

    Object& getBallProjection();

    Object& getBall();

    vector<Object>& getTeam();

    vector<Object>& getOpponent();

    Object& getRobot(int);

    Object& getAttackRobot();

    Object& getDefenseRobot();

    Object& getGoalRobot();

    Point& getImageSize();

    Point& getGoalSize();

    Point& getGoalArea();

    /**
     * Saves all the targets in a vector for drawing purposes
     * \param id: the robot id
     * \param target: the robot target
     */
    void setTargetOf(int, Point);

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

private:
    Strategy(); ///< private constructor for singleton

    void defineFunctionsForEachRobot();

    static Strategy* instance; ///< holds the class instance

    Object ball; ///< contain the ball coordinates
    vector<Object> team; ///< vector containing the team's positions
    vector<Object> opponent;
    vector<Object> objects; ///< vector of all objects containing its positions
    vector<Point> targets;

    Object ballProjection;
    vector<Object> lastBallPositions;
    Object lastBallProjection;

    StrategyFactory* strategyAttack;
    StrategyFactory* strategyDefense;
    StrategyFactory* strategyGoal;

    float curve_factor;
    float potency_factor;

    Point imageSize;
    Point goalSize;
    Point goalArea;

};

#endif