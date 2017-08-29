#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){}

void Strategy::initializeStrategies(){
    strategies["attack"] = new StrategyAttack();
//    strategies["defense"] = new StrategyDefense();
//    strategies["goal"] = new StrategyGoal(); 
}

Strategy* Strategy::getInstance(){
    
    if(instance == NULL){
        instance = new Strategy();
        instance->initializeStrategies();
    }

    return instance;
}

void Strategy::defineFunctionsForEachRobot(vector<Robot>& robots){
     // fazer if em relacao a quantidade do vetor team
    if(robots[1].distanceFrom(ball) < robots[0].distanceFrom(ball)){
        team["attack"] = (robots[1]);
        team["defense"] = (robots[0]);
    } else {
        team["attack"] = (robots[0]);
        team["defense"] = (robots[1]);
    }  

    team["goal"] = robots[2];

//    strategies["attack"]->setRobot();
//   strategies["defense"]->setRobot(team[1]);
//   strategies["goal"]->setRobot(team[2]);
}

void Strategy::apply(vector<Robot>& _team, vector<Robot> _opponent, Ball _ball){
    // team.assign(_team.begin(), _team.end());
    opponent.swap(_opponent);
    ball = _ball; 

    defineFunctionsForEachRobot(_team);

    cout << "APLICAR ESTRATEGIA" << endl;
    for(auto it = strategies.begin() ; it != strategies.end() ; it++){
        string function = it->first;
        (it)->second->apply(team[function]);
    }
}

Ball& Strategy::getBall(){
    return ball;
}

Robot Strategy::getRobot(string func){
    return strategies[func]->getRobot();
}

map<string, Robot>::iterator Strategy::getRobotsBegin(){
    return team.begin();
}

map<string, Robot>::iterator Strategy::getRobotsEnd(){
    return team.end();
}