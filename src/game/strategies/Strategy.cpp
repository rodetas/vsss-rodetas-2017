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

//   strategies["attack"]->setRobot();
//   strategies["defense"]->setRobot(team[1]);
//   strategies["goal"]->setRobot(team[2]);
}

void Strategy::apply(vector<Robot>& _team, vector<Robot> _opponent, Ball _ball){
    opponent.swap(_opponent);
    ball = _ball; 

    defineFunctionsForEachRobot(_team);

    for(auto it = strategies.begin() ; it != strategies.end() ; it++){
        string function = it->first;
        (it)->second->apply(team[function]);
    }

    robots.clear();
    for(auto it=team.begin() ; it!=team.end() ; it++){
        robots.push_back(it->second);
    }
}

Ball& Strategy::getBall(){
    return ball;
}

Robot Strategy::getRobot(string func){
    return team[func];
}

vector<Robot>::iterator Strategy::getRobotsBegin(){
    return robots.begin();
}

vector<Robot>::iterator Strategy::getRobotsEnd(){
    return robots.end();
}