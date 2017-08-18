#ifndef STRATEGY_HPP_
#define STRATEGY_HPP_

#include "../../Header.h"
#include "../../utils/Commons.h"
#include "../Movimentation.h"
#include "../../utils/Manipulation.h"

using namespace rodetas;

enum {PARADO, DEFENDENDO, ATACANDO};

class Strategy;

class StrategyFactory {

public:

    /**
     * Default constructor
     */
    StrategyFactory();

    /**
     * Constructor - if uses this constructor once, should use this for all
     * \param n: robot id for use
     */
    StrategyFactory(int n);

    /**
     * Method to initialize strategy's parameters
     */
    void initialize();

    /**
     * Static Method to initialize all the static parameters
     */
    static void initStaticParameters();

    /**
     * Static method to define positions for each robot
     */
    static void defineFunctions();

    /**
     * Pure virtual function to apply the strategy
     */
    virtual void apply() = 0;

    /**
     * Pure virtual function to define where goes the robot
     */
    virtual Point setTarget() = 0;

    /**
     * Apply the robot strategy when it's on the corner
     */
    virtual void cornerStrategy();

    /**
     * Returns if a rodetas::object is near the board
     * \param the object you want to know if is near the board
     * \return true if is near
     */
    bool isBoard(rodetas::Object);

    /**
     * Apply the potencial field
     * \param target: 
     * \param toRepulsion:
     * \param toDestination:
     * \return 
     */
     // @TODO: ADICIONAR A DOCUMENTACAO DESTE METODO
    virtual Point applyPotencialField(Point target, rodetas::Object toRepulsion, rodetas::Object toDestination);

    /**
     * Updates the calculus between robot, ball and target
     */
    void updateCalculus();

    /**
     * Calculates the ball projection
     * \return the ball projection
     */
    static Object calculateBallProjection();

    /**
     * Method to get the command generate by strategy for a specific robot
     * \return the respectively command
     */
    Command getCommand();

    /**
     * Get the robot id whose the strategy is using
     * \return the robot id
     */
    int getRobotId();
    
protected:

    Strategy* data;

    static int attackNumber; ///< index of the attacker robot in the team vector
    static int defenseNumber;///< index of the defenser robot in the team vector
    static int goalNumber;   ///< index of the goalkeeper robot in the team vector

    static Point imageSize;
    static Point goalSize;
    static Point goalArea;

    static float curve_factor;
    static float potency_factor;

    int robotState; ///< for represent the atual robot state (PARADO, DEFENDENDO, ATACANDO)
    int robotId;

    Point destination;

    Object robot;

    float distance_robot_ball;
    float distance_robot_destination;
    float distance_ball_destination;
    float angle_robot_destination;
    float sin_robot_destination;
    float cos_robot_destination;
    float cos_robot_ball;
    float convertRadiansToDegrees;

    Movimentation movimentation;

private:

    static int nStrategies; ///< number of robots strategy will deal 
    static bool definedId;

};

#include "Strategy.h"
    
#endif