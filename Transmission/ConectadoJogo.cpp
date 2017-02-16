#include "ConectadoJogo.h"

ConectadoJogo::ConectadoJogo() : Transmission(){
    openConection();
}

ConectadoJogo::~ConectadoJogo(){
    stopRobot();
    Transmission::closeConnection();
}

void ConectadoJogo::send(){    
    string comand = "";
    for(int i=0; i<3 ; i++){
        comand += Transmission::generateChecksum(i, movements[i]);
    }

    Transmission::transmitting(comand);
}

void ConectadoJogo::movementRobot(Command c){
    string message = "";
    for(int i=0 ; i<3 ; i++)
        message += Transmission::generateChecksum(i,c);

    Transmission::transmitting(message);
}

void ConectadoJogo::stopRobot(){
    movementRobot(Command(STOPPED_MOVE,0,0));
}

void ConectadoJogo::setMovements(vector<Command> mov){
    swap(movements, mov); // movements = mov;
}