#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "../../utils/Commons.h"

using namespace rodetas;

class Strategy{

public:

    // Singleton Implementation
    static Strategy* getInstance();

    void apply(); // analisar a necessidade de retornar vector de command

    void setPotencyFactor(float);
    void setCurveFactor(float);

private:
    Strategy();

    static Strategy* instance;

    Object ball; ///< contain the ball coordinates
    vector<rodetas::Object> team; ///< vector containing the team's positions
    vector<rodetas::Object> opponent;
    vector<rodetas::Object> objects; ///< vector of all objects containing its positions
    vector<Point> targets;

};

#endif