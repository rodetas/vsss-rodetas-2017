#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include "../utils/Timer.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

using namespace std;
using namespace rodetas;

class Transmission {

private:
    Timer timer;
    int usb;
    int last_time;
    bool status;
    bool openStatus;
    char initialCaracter[3];
    char finalCaracter[3];

    string robot_speed;
    bool receiving;
    
protected:
    vector<Command> movements;

public:
	Transmission();
    virtual ~Transmission();

    bool openConection();
    bool getConnectionStatus();
    void closeConnection();

    void reading();
    void transmitting(string);

    void readChecksum();
    string generateMessage(int, Command);

    void setMovements(vector<Command>);
};
#endif