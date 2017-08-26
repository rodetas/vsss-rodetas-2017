#ifndef STRATEGY_DEFENSE_H_
#define STRATEGY_DEFENSE_H_

#include "StrategyFactory.h"

class StrategyDefense : public StrategyFactory {

public:
    StrategyDefense();

    virtual void apply(int id);
    virtual Point setTarget();

};

#endif