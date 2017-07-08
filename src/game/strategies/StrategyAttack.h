#ifndef ATTACK_STRATEGY_H_
#define ATTACK_STRATEGY_H_

#include "StrategyFactory.h"
#include "../../utils/Commons.h"

class StrategyAttack : public StrategyFactory {

public:

    StrategyAttack();

    virtual void apply();
    virtual Point setTarget();
    
};

#endif