#ifndef ATTACK_STRATEGY_H_
#define ATTACK_STRATEGY_H_

#include "StrategyBase.h"
#include "../../utils/Commons.h"

class StrategyAttack : public StrategyBase {

public:

    StrategyAttack();

    virtual void apply();
    virtual Point defineTarget();
    
};

#endif