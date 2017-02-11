#include "ConectadoJogo.h"

ConectadoJogo::ConectadoJogo() : Transmission(){
    openConection();
}

ConectadoJogo::~ConectadoJogo(){
    Command c(STOPPED_MOVE,0,0);
    string envia = "";

    for(int i=0 ; i<3 ; i++)
        envia += Transmission::geraStringComando(i,c);

    Transmission::transmite(envia);

    Transmission::closeConnection();
}

void ConectadoJogo::send(){
    
    string comand = "";
    for(int i=0; i<3 ; i++){
        comand += Transmission::geraStringComando(i, movements[i]);
    }

    Transmission::transmite(comand);
}