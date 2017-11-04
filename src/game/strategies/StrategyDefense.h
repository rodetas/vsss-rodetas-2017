#ifndef STRATEGY_DEFENSE_H_
#define STRATEGY_DEFENSE_H_

#include "StrategyBase.h"

class StrategyDefense : public StrategyBase {

public:
    StrategyDefense();

    virtual Command strategy(Robot*, Command);
    virtual Command collisionStrategy(Command);    
    virtual Point defineTarget(Robot*);

};

#endif