#ifndef STRATEGY_GOAL_H_
#define STRATEGY_GOAL_H_

#include "StrategyFactory.h"

class StrategyGoal : public StrategyFactory {

public:
    StrategyGoal();

    virtual void apply();
    virtual Point setTarget();

};

#endif