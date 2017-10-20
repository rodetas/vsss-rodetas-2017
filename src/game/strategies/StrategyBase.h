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

     virtual void apply(Robot*);
     
     /**
      * Pure virtual function to apply the strategy
      */
    virtual Command strategy(Robot*, Command) = 0;

    /**
     * Pure virtual function to define where goes the robot
     */
    virtual Point defineTarget(Robot*) = 0;

    /**
     * Apply the robot strategy when it's on the corner
     */
    virtual void cornerStrategy();

    virtual Command stopStrategy(Command);

     /**
     * Apply the potencial field
     * \param target: 
     * \param toRepulsion:
     * \param toDestination:
     * \return 
     */
     // @TODO: ADICIONAR A DOCUMENTACAO DESTE METODO
    virtual Point applyPotencialField(const Point& target, const Point& toRepulsion, const Point& toDestination) const;

    void setRobot(Robot*);

    Robot* getRobot();

    static int getNumStrategies();

protected:

    Strategy* data;
    
    static float curve_factor;
    static float potency_factor;

    int robotState; ///< for represent the atual robot state (PARADO, DEFENDENDO, ATACANDO)

    Robot* robot;

    Movimentation movimentation;

private:
    static int nStrategies; ///< number of robots strategy will deal 
        
};

#include "Strategy.h"

#endif