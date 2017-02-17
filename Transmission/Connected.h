#ifndef TRANSMISSION_JOGO
#define TRANSMISSION_JOGO

#include "../utils/Structs.h"
#include "Transmission.h"

class Connected : public Transmission{

    private:

    public:
        Connected();
        virtual ~Connected();
        void send();

        void stopRobot();
        void movementRobot(Command);

};

#endif