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
}

Transmission::~Transmission(){
    closeTransmission();
    closeConnection();
}

int Transmission::closeTransmission(){
    for(int i=0 ; i<3 ; i++) { 
        sendMovement(i, STOPPED_MOVE,0); 
    }
    return EXIT;
}   

void Transmission::closeConnection(){
    close(usb);   
}

bool Transmission::openConection(){   

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

// recebe um comando e retorna a string equivalente para ser enviada
string Transmission::geraStringComando(int robot, Command comand){

    stringstream ss, ss1;
    ss << setfill('0') << setw(3) << comand.pwm1;
    ss1 << setfill('0') << setw(3) << comand.pwm2;

    string pwm_str = comand.direcao + ss.str() + ss1.str();

    int checksum = 0; // gera a soma acumulada do checksum
    for(int i=0 ; i<pwm_str.size() ; i++){
        checksum += int(pwm_str[i]);
    }

    checksum += int(initialCaracter[robot]); // adiciona aos caracteres inicial e final do robo
    checksum += int(finalCaracter[robot]);

    string checksumString = to_string(checksum);    //converte para string

    // retorna a string pronta para ser enviada
    string comando_str =    initialCaracter[robot] + pwm_str +
                            checksumString + finalCaracter[robot];

    return comando_str;
}

// recebe uma string para enviar
void Transmission::transmite(string comand){
    const int size = comand.size();
    unsigned char send_bytes[size];

    if(time > 60){
        string out = executeCommand("ls /dev/ttyUSB0 2> /dev/null");

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

        //cout << send_bytes << endl;

        write(usb, send_bytes, size);
    }
}

bool Transmission::getConnectionStatus(){
    return status && openStatus;
}

void Transmission::setMovements(vector<Command> mov){
    swap(movements, mov); // movements = mov;
}














void Transmission::sendMovement(int robot, char comand, int power){
    string movementConfigure;
    string stopped = "000000";

    if (power == 0){
        movementConfigure = STOPPED_MOVE + stopped; 
    } else {
        movementConfigure = comand + to_string(power);
    }

    //transmite( checksum(robot, movementConfigure) );
}
