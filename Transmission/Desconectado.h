#ifndef DESCONECTADO
#define DESCONECTADO

#include "Transmission.h"
#include "../utils/Structs.h"

class Desconectado : public Transmission {
    private:

    public:
        Desconectado();
        virtual ~Desconectado();
        virtual void send();
};

#endif