#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){}

void Strategy::initializeStrategies(){
    strategies["attack"] = new StrategyAttack();
    strategies["defense"] = new StrategyDefense();
    //strategies["goal"] = new StrategyGoal(); 
}

Strategy* Strategy::getInstance(){
    
    if(instance == NULL){
        instance = new Strategy();
        instance->initializeStrategies();
    }

    return instance;
}

void Strategy::apply(vector<Robot>& _team, vector<Robot> _opponent, Ball _ball){
    team.swap(_team);
    opponent.swap(_opponent);
    ball = _ball;

    // fazer if em relacao a quantidade do vetor team
   /*  if (distance(team[GRAPHICPLAYER1], ball) < distance(team[GRAPHICPLAYER0], ball)){
        //strategyAttack->apply(1);
        //strategyDefense->apply(0);
    } else {
        //strategyAttack->apply(0);
        //strategyDefense->apply(1);
    } */	

    //strategyGoal->apply(2);
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