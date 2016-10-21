#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_

#include "Header.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include "utils/Structs.h"

using namespace std;
using namespace rod;

class Transmission{
private:
    int usb;
    int time;
    bool status;
    bool openStatus;
    char initialCaracter[3];
    char finalCaracter[3];
public:
	Transmission();
    void openConection();
    void sendMovement(int, string, int);
    void transmitting(string);
    string checksum(int, string);
    void setMovements(vector<string>);
    bool getConnectionStatus();
    void closeConnection();
};
#endif