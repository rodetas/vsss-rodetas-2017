#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){}

void Strategy::initializeStrategies(){
    strategies["attack"] = new StrategyAttack();
    strategies["defense"] = new StrategyDefense();
    strategies["goal"] = new StrategyGoal(); 
}

Strategy* Strategy::getInstance(){
    
    if(instance == NULL){
        instance = new Strategy();
        instance->initializeStrategies();
    }

    return instance;
}

void Strategy::defineFunctionsForEachRobot(){
    // fazer if em relacao a quantidade do vetor team
    if(team[1].distanceFrom(ball) < team[0].distanceFrom(ball)){
        strategies["attack"]->setRobot(team[1]);
        strategies["defense"]->setRobot(team[0]);
    } else {
        strategies["attack"]->setRobot(team[0]);
        strategies["defense"]->setRobot(team[1]);
    } 

    strategies["goal"]->setRobot(team[2]);
}

void Strategy::apply(vector<Robot>& _team, vector<Robot> _opponent, Ball _ball){
    team.swap(_team);
    opponent.swap(_opponent);
    ball = _ball;

    defineFunctionsForEachRobot();

    for(auto it = strategies.begin() ; it != strategies.end() ; it++){
        (it)->second->apply();
    }
}

Ball& Strategy::getBall(){
    return ball;
}

vector<Robot>& Strategy::getTeam(){
    return team;
}

vector<Robot>& Strategy::getOpponent(){
    return opponent;
}

Robot Strategy::getRobot(string func){
    return strategies[func]->getRobot();
}

vector<Robot>::iterator Strategy::getRobotsBegin(){
    return team.begin();
}

vector<Robot>::iterator Strategy::getRobotsEnd(){
    return team.end();
}