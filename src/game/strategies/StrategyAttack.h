#ifndef ATTACK_STRATEGY_H_
#define ATTACK_STRATEGY_H_

#include "StrategyBase.h"
#include "../../utils/Commons.h"

class StrategyAttack : public StrategyBase {

public:

    StrategyAttack();

    virtual Command strategy(const Robot&, Command);
    virtual Point defineTarget(const Robot&);
    
};

#endif