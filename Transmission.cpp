#include "Transmission.h"

Transmission::Transmission() {
    initialCaracter[0] = robot0InitialCharacter;
    initialCaracter[1] = robot1InitialCharacter;
    initialCaracter[2] = robot2InitialCharacter;
    finalCaracter[0] = robot0FinalCharacter;
    finalCaracter[1] = robot1FinalCharacter;
    finalCaracter[2] = robot2FinalCharacter;

    time = 0;
    status = false;
    openStatus = false;

    openConection();

}    


void Transmission::closeConnection(){
    close(usb);
}

void Transmission::openConection(){   

    usb = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (usb == -1 ) {
        openStatus = false;

    } else {

        struct termios options;
        memset (&options, 0, sizeof options);

        /* Error Handling */
        if (tcgetattr(usb, &options) != 0 ) {
            //cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
            openStatus = false;
        } else {
            openStatus = true;
        }

        /* Set Baud Rate */
        cfsetospeed(&options, (speed_t)B19200);
        cfsetispeed(&options, (speed_t)B19200);

        /* Setting other Port Stuff */
        options.c_cflag     &=  ~PARENB;            // Make 8n1
        options.c_cflag     &=  ~CSTOPB;
        options.c_cflag     &=  ~CSIZE;
        options.c_cflag     |=  CS8;
        options.c_cflag     &=  ~CRTSCTS;           // no flow control
        options.c_cc[VMIN]   =  1;                  // read doesn't block
        options.c_cc[VTIME]  =  5;                  // 0.5 seconds read timeout
        options.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

        /* Make raw */
        cfmakeraw(&options);

        /* Flush Port, then applies attributes */
        tcflush(usb, TCIFLUSH);

        if(tcsetattr(usb, TCSANOW, &options) != 0) {
            //cout << "Error " << errno << " from tcsetattr: " << strerror(errno) << endl;
            openStatus = false;
        } else {
            openStatus = true;
        }
    }
}

void Transmission::setMovements(vector<string> movements){
    string comand = "";
    for(int i=0; i<3 ; i++){
        string movement = movements[i];
        comand = comand + checksum(i, movement);
    }

    transmitting(comand);
}

string Transmission::checksum(int robot, string comand){
    int checksum = 0;

    for (int i = 0; i < comand.size(); i++){
        checksum = checksum + int(comand[i]);
    }

    checksum = checksum + int(initialCaracter[robot]);    
    checksum = checksum + int(finalCaracter[robot]);

    string checksumString = to_string(checksum);
 
    comand = initialCaracter[robot] + comand + checksumString + finalCaracter[robot];

    return comand;
}

void Transmission::transmitting(string comand){
    const int size = comand.size();
    unsigned char send_bytes[size];

    if(time > 60){
        string out = executeCommand("ls /dev/ttyUSB0");

        if(out.compare("") == 0){
            status = false;
        } else {
            status = true;
        }
        time = 0;

    } else {
        time++;
    }

    if(status == false || openStatus == false){
        closeConnection();
        openConection();

    } else {

        for (int i = 0; i < size; i++) {
            if (i < comand.size())
                send_bytes[i] = comand[i];
            else
            send_bytes[i] = ' ';
        }

       // cout << send_bytes << endl;

        write(usb, send_bytes, size);
    }
}

void Transmission::sendMovement(int robot, string comand, int power){
    string movementConfigure;
    string stopped = "000000";

    if (power == 0){
        movementConfigure = STOPPED_MOVE + stopped; 
    } else {
        movementConfigure = comand + to_string(power);
    }

    transmitting( checksum(robot, movementConfigure) );
}

bool Transmission::getConnectionStatus(){
    return status && openStatus;
}