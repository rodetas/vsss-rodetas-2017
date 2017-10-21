#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){
    potency_factor = 1.2;
    curve_factor = 1.1;
}

Strategy* Strategy::getInstance(){
    
    if(instance == NULL){
        instance = new Strategy();
        instance->initializeStrategies();
    }

    return instance;
}

void Strategy::initializeStrategies(){
    strategies["attack"] = new StrategyAttack();
    strategies["defense"] = new StrategyDefense();
    strategies["goal"] = new StrategyGoal(); 
}

void Strategy::defineFunctionsForEachRobot(vector<Robot>& robots){
     // fazer if em relacao a quantidade do vetor team
    /* if(robots[1].distanceFrom(ball) < robots[0].distanceFrom(ball)){
        team["attack"] = (robots[1]);
        team["defense"] = (robots[0]);
    } else {
        team["attack"] = (robots[0]);
        team["defense"] = (robots[1]);
    }   */

    team["attack"] = &robots[1];
    team["defense"] = &robots[0];
    team["goal"] = &robots[2];
}

void Strategy::apply(vector<Robot>& _team, vector<Robot> _opponent, Ball _ball){
    opponent.swap(_opponent);
    ball = _ball; 

    // define as funcoes de cada robo
    defineFunctionsForEachRobot(_team);

    // aplica a estrategia para cada robo
    for(auto it = strategies.begin() ; it != strategies.end() ; it++){
        string function = it->first;
        // envia o robo especifico como parametro para a estrategia
        (it)->second->apply(team[function]);
    }

    robots.clear();
    for(auto it=team.begin() ; it!=team.end() ; it++){
        robots.push_back(*(it->second));
    }
}

Ball& Strategy::getBall(){
    return ball;
}

Robot Strategy::getRobot(string func){
    return *(team[func]);
}

vector<Robot>::iterator Strategy::getRobotsBegin(){
    return robots.begin();
}

vector<Robot>::iterator Strategy::getRobotsEnd(){
    return robots.end();
}

void Strategy::setPotencyFactor(float pFactor){
    potency_factor = pFactor;
}
     
void Strategy::setCurveFactor(float cFactor){
    curve_factor = cFactor;
}

float Strategy::getPotencyFactor(){
    return potency_factor;
}
    
float Strategy::getCurveFactor(){
    return curve_factor;
}

vector<Point> Strategy::getTargets(){
    vector<Point> targets;

    for(auto it=team.begin() ; it!=team.end() ; it++){
        targets.push_back((it)->second->getTarget());
    }

    return targets;
}

