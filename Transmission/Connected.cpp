#include "Connected.h"

Connected::Connected() : Transmission(){
    openConection();
}

Connected::~Connected(){
    stopRobot();
    Transmission::closeConnection();
}

void Connected::send(int i, Command c){    
    string comand = "";
    cout << i << " " << c.to_string() << endl;
    comand = Transmission::generateMessage(i, c);
    Transmission::transmitting(comand);
}

void Connected::movementRobot(Command c){
    string message = "";
    for(int i=0 ; i<3 ; i++)
        message += Transmission::generateMessage(i,c);

    Transmission::transmitting(message);
}

void Connected::stopRobot(){
    movementRobot(Command(STOPPED_MOVE,0,0));
}