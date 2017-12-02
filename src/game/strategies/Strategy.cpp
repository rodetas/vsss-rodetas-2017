#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){
    timeLastChange = -1;
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

void Strategy::initializeRobots(vector<Robot*> _robots){
    team["attack"] = _robots[1];
    team["defense"] = _robots[0];
    team["goal"] = _robots[2];
}

void Strategy::defineFunctionsForEachRobot(vector<Robot*> _robots){

    if(timeLastChange == -1){
        float distanceDefenseBall = distance(team["defense"]->getPosition(), ball->getPosition());
        float distanceAttackBall = distance(team["attack"]->getPosition(), ball->getPosition());
        Point ballProjection = ball->getBallProjection();


        if (team["attack"]->x() > ball->x() * 1.05 && !(team["attack"]->x() > ball->x() && team["defense"]->x() > ball->x()) && 
            !team["attack"]->isBlocked() && !team["defense"]->isBlocked()){

            Robot *aux = team["attack"];
            team["attack"] = team["defense"];
            team["defense"] = aux;
        }

        // na defesa, o mais perto é o atacante
        if(distanceDefenseBall < distanceAttackBall && ball->x() < imageSize.x/2 && 
            !team["attack"]->isBlocked() && !team["defense"]->isBlocked()){
            Robot *aux = team["attack"];
            team["attack"] = team["defense"];
            team["defense"] = aux;
        }

        if(team["attack"]->isBlocked()){
            Robot *aux = team["attack"];
            team["attack"] = team["defense"];
            team["defense"] = aux;
        }

        timeLastChange = 60;

        /*
        int halfGoal1 = rodetas::imageSize.y/2 + (rodetas::goalSize.y)*0.4;
        int halfGoal2 = rodetas::imageSize.y/2 - (rodetas::goalSize.y)*0.4;
         if((ballProjection.y > halfGoal1 || ballProjection.y < halfGoal2) && ballProjection.x < imageSize.x*0.20 &&
            distance(team["attack"]->getPosition(), ball->getPosition()) > 200){
                Robot *aux = team["attack"];
                team["attack"] = team["goal"];
                team["goal"] = team["defense"];
                team["defense"] = aux;
                
                timeLastChange = 300;
        } */

    } 

    if(timeLastChange >= 0) timeLastChange--;
}

void Strategy::apply(vector<Robot*> _team, vector<Robot*> _opponent, Ball* _ball, bool targetFromScreen, bool play){
    opponent = (_opponent);
    ball = _ball; 

    if(!targetFromScreen){

        // define as funcoes de cada robo
        if(play){
            defineFunctionsForEachRobot(_team);
        } else {
            timeLastChange = 60;
        }

        // aplica a estrategia para cada robo
        for(auto it = strategies.begin() ; it != strategies.end() ; it++){
            string function = it->first;
            // envia o robo especifico como parametro para a estrategia
            (it)->second->apply(team[function]);
        }

    } else {

        for(auto it = strategies.begin() ; it!=strategies.end() ; it++){
            string function = it->first;
            // envia o robo especifico como parametro para a estrategia
            (it)->second->move(team[function]);
        }
    }

}

Ball* Strategy::getBall(){
    return ball;
}

Robot Strategy::getRobot(string func){
    return *(team[func]);
}

void Strategy::changeAttack(){
    Robot *aux = team["attack"];
    team["attack"] = team["defense"];
    team["defense"] = aux;
}