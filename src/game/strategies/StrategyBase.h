#ifndef STRATEGY_HPP_
#define STRATEGY_HPP_

#include "../../Header.h"
#include "../../utils/Commons.h"
#include "../../utils/Robot.h"
#include "../Movimentation.h"
#include "../../utils/Manipulation.h"

using namespace rodetas;

enum {PARADO, DEFENDENDO, ATACANDO};

class Strategy;

class StrategyBase {
    
public:
    /**
     * Default constructor
     */
     StrategyBase();

     virtual void apply();
     
     /**
      * Pure virtual function to apply the strategy
      */
    virtual void strategy() = 0;

    /**
     * Pure virtual function to define where goes the robot
     */
    virtual Point defineTarget() = 0;

    /**
     * Apply the robot strategy when it's on the corner
     */
    virtual void cornerStrategy();

    void setRobot(Robot);

     /**
     * Apply the potencial field
     * \param target: 
     * \param toRepulsion:
     * \param toDestination:
     * \return 
     */
     // @TODO: ADICIONAR A DOCUMENTACAO DESTE METODO
    virtual Point applyPotencialField(Point target, Point toRepulsion, Point toDestination);

    static int getNumStrategies();

protected:

    Strategy* data;
    
    static float curve_factor;
    static float potency_factor;

    int robotState; ///< for represent the atual robot state (PARADO, DEFENDENDO, ATACANDO)

    Robot robot;

    Movimentation movimentation;

private:
    static int nStrategies; ///< number of robots strategy will deal 
        
};

#include "Strategy.h"

#endif