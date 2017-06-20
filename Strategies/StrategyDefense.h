#ifndef STRATEGY_DEFENSE_H_
#define STRATEGY_DEFENSE_H_

#include "Strategy.h"

class StrategyDefense : public Strategy {

public:
    StrategyDefense();

    virtual void apply();
    virtual Point setTarget();

};

#endif