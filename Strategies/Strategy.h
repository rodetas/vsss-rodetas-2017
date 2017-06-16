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
     * n: robot id for use
     */
    Strategy(int n);

    /**
     * Method to initialize strategy's parameters
     */
    void initialize();

    /**
     *
     */
    static void initStaticParameters();

    /**
     *    Static method to define positions for each robot
     */
    static void defineFunctions();

    /**
     *   Pure virtual function to apply the strategy
     */
    virtual void apply() = 0;

    /**
     * Pure virtual function to define where goes the robot
     */
    virtual Point setTarget() = 0;

    /**
     * 
     */
    virtual void cornerStrategy();

    /**
     * Returns if a rodetas::object is near the board
     */
    bool isBoard(rodetas::Object);

    /**
     * Apply the potencial field
     * target: 
     * toRepulsion:
     * toDestination:
     */
     // @TODO: ADICIONAR A DOCUMENTACAO DESTE METODO
    virtual Point applyPotencialField(Point target, rodetas::Object toRepulsion, rodetas::Object toDestination);

    /**
     * Saves all the targets in a vector for drawing purposes
     * id: the robot id
     * target: the robot target
     */
    static void setVecTarget(int id, Point target);

    /**
     * Updates the calculus between robot, ball and target
     */
    void updateCalculus();

    /**
     *
     */
    static void calcBallProjection();

    static void setObjects(const vector<rodetas::Object>&, const vector<rodetas::Object>&, rodetas::Object);

    /**
     *
     */
    Command getCommand();

    int getRobotId();

protected:

    static int attackNumber;
    static int defenseNumber;
    static int goalNumber;

    static Object ball;
    static vector<rodetas::Object> team;
    static vector<rodetas::Object> opponent;
    static vector<rodetas::Object> objects; ///< vector of all objects containing its positions
    static vector<Point> targets;

    static Object ballProjection;
    //@TODO: imeplementar uma melhor projecao da bola
    static vector<rodetas::Object> lastBallPositions;
    static rodetas::Object lastBallProjection;

    static Point imageSize;
    static Point goalSize;
    static Point goalArea;

    int robotState;
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