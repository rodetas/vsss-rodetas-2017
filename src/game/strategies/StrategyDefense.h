#ifndef STRATEGY_DEFENSE_H_
#define STRATEGY_DEFENSE_H_

#include "StrategyFactory.h"

class StrategyDefense : public StrategyFactory {

public:
    StrategyDefense();

    virtual void apply();
    virtual Point setTarget();

};

#endif