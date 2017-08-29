#ifndef STRATEGY_GOAL_H_
#define STRATEGY_GOAL_H_

#include "StrategyBase.h"

class StrategyGoal : public StrategyBase {

public:
    StrategyGoal();

    virtual Command strategy(const Robot&, Command);
    virtual Point defineTarget(const Robot&);

};

#endif