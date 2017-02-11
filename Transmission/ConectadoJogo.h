#ifndef TRANSMISSION_JOGO
#define TRANSMISSION_JOGO

#include "../utils/Structs.h"
#include "Transmission.h"

class ConectadoJogo : public Transmission{

    private:


    public:
        ConectadoJogo();
        virtual ~ConectadoJogo();
        virtual void send();    

};

#endif