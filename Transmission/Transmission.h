#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_

#include "../Header.h"
#include "../utils/Structs.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

using namespace std;
using namespace rodetas;

class Transmission {

private:
    int usb;
    int time;
    bool status;
    bool openStatus;
    char initialCaracter[3];
    char finalCaracter[3];

protected:
    vector<Command> movements;

public:
	Transmission();
    virtual ~Transmission();

    virtual void send() = 0;

    bool openConection();

    void sendMovement(int, char, int);
    void transmite(string);
    string geraStringComando(int, Command);
    bool getConnectionStatus();
    void closeConnection();
    int closeTransmission();

    void setMovements(vector<Command>);
};
#endif