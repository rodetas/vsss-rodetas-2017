#ifndef ATTACK_STRATEGY_H_
#define ATTACK_STRATEGY_H_

#include "StrategyBase.h"
#include "../../utils/Commons.h"

class StrategyAttack : public StrategyBase {

public:

    StrategyAttack();

    virtual Command strategy(Robot*, Command);
    virtual Point defineTarget(Robot*);
    
};

#endif