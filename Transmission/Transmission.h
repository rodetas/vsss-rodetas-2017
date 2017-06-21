#ifndef TRANSMISSION_H_
#define TRANSMISSION_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include "../utils/Timer.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <bitset>
#include <mutex>

using namespace std;
using namespace rodetas;

class Transmission {

private:
    Timer timer;
    int usb;
    int last_time;
    bool transmittingStatus;
    bool openStatus;
    char initialCaracter[3];
    char finalCaracter[3];

    string robot_speed;
    bool receiving;

    std::mutex mutex;

public:
	Transmission();

    virtual ~Transmission();

    /**
     * Close the serial connection
     */
    void closeConnection();
    
    /**
     * Open the serial connection with xbee
     * \return true if connection succeed
     */
    bool openConection();

    /**
     * Send a command to a specific robot
     * /param id: the robot address
     * /param c: the command to send
     */
    void send(int id, Command c);

    /**
     * Method to generate a string to be send
     * \param id: the robot id
     * \param c: the command to send
     * \return the prepared string
     */
    string generateMessage(int id, Command c);

    /**
     * Generated the checksum correspondent; The 8-bit sum of the parameters
     * \param frame: the message frame type
     * \param id: the frame id
     * \param address: the robot address
     * \param option: the message option parameter
     * \param hex: the vector of int containing the message values
     * \return the respectively checksum value
     */
    int generateCheckSum(int frame, int id, int address, int option, vector<int> hex);

    /**
     * Send via xbee the message
     * \param receives the message ready to be send
     */
    void serialTransmit(string);

    
    /**
     * Send stop command to a specific number of robots
     * \param the number of robots to send
     */
    void stopAllRobots(int number);

    /**
     * \return true if connection is established
     */
    bool getConnectionStatus();

};
#endif