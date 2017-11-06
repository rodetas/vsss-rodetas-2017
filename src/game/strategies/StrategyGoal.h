#ifndef STRATEGY_GOAL_H_
#define STRATEGY_GOAL_H_

#include "StrategyBase.h"

class StrategyGoal : public StrategyBase {

public:
    StrategyGoal();

    virtual void apply(Robot*);
    virtual Command strategy(Robot*, Command);
    virtual Point defineTarget(Robot*);
    virtual Command stopStrategy(Command);

};

#endif