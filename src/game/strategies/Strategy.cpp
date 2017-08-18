#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){
	/* strategies.push_back(new StrategyAttack());
	strategies.push_back(new StrategyDefense());
	strategies.push_back(new StrategyGoal()); */
}

Strategy* Strategy::getInstance(){

    if(instance == NULL){
        instance = new Strategy();
    }

    return instance;
}

void Strategy::apply(vector<Object>& team, vector<Object>& opponent, Object& ball){

    StrategyFactory::defineFunctions();

    for(int i=0 ; i<strategies.size() ; i++){
        strategies[i]->apply();
    }

}

int Strategy::getNumStrategies(){
    return strategies.size();
}

Object& Strategy::getBall(){
    return ball;
}

void Strategy::setTargetOf(int id, Point target){
	targets[id] = target;
}

void Strategy::setPotencyFactor(float p){
    potency_factor = p;
}

void Strategy::setCurveFactor(float p){
    curve_factor = p;
}