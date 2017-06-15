#ifndef STRATEGY_GOAL_H_
#define STRATEGY_GOAL_H_

#include "Strategy.h"

class StrategyGoal : public Strategy {

public:
    StrategyGoal();

    virtual void apply();
    virtual Point setTarget();

};

#endif