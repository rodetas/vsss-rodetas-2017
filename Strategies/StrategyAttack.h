#ifndef ATTACK_STRATEGY_H_
#define ATTACK_STRATEGY_H_

#include "Strategy.h"
#include "../utils/Structs.h"

class StrategyAttack : public Strategy {

public:

    StrategyAttack();

    virtual void apply();
    virtual Point setTarget();
    
};

#endif