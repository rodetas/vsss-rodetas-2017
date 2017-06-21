#ifndef STRATEGY_HPP_
#define STRATEGY_HPP_

#include "../Header.h"
#include "../utils/Structs.h"
#include "../Movimentation.h"
#include "../CRUD/Manipulation.h"

using namespace rodetas;

enum {PARADO, DEFENDENDO, ATACANDO};

class Strategy {

public:

    /**
     * Default constructor
     */
    Strategy();

    /**
     * Constructor - if uses this constructor once, should use this for all
     * \param n: robot id for use
     */
    Strategy(int n);

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
     * Saves all the targets in a vector for drawing purposes
     * \param id: the robot id
     * \param target: the robot target
     */
    static void setVecTarget(int id, Point target);

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
     * Sets the objects for strategy's use
     * \param team: receives a vector<Object> containing all the team positions
     * \param opponent: receives a vector<Object> containing the opponent's positions
     * \param ball: receive a Object containg the ball's position
     */
    static void setObjects(const vector<rodetas::Object>&, const vector<rodetas::Object>&, rodetas::Object);

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

    /**
     * Sets the potency factor value
     * \param f: the potency
     */
    static void setPotencyFactor(float f);

    /**
     * Sets the curve factor
     * \param f: the curve factor
     */
    static void setCurveFactor(float f);
    

protected:

    static int attackNumber; ///< index of the attacker robot in the team vector
    static int defenseNumber;///< index of the defenser robot in the team vector
    static int goalNumber;   ///< index of the goalkeeper robot in the team vector

    static Object ball; ///< contain the ball coordinates
    static vector<rodetas::Object> team; ///< vector containing the team's positions
    static vector<rodetas::Object> opponent;
    static vector<rodetas::Object> objects; ///< vector of all objects containing its positions
    static vector<Point> targets;

    static Object ballProjection;
    static vector<rodetas::Object> lastBallPositions;
    static rodetas::Object lastBallProjection;

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


    
#endif