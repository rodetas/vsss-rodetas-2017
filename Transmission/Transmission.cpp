#include "Transmission.h"

Transmission::Transmission() {
    initialCaracter[0] = robot0InitialCharacter;
    initialCaracter[1] = robot1InitialCharacter;
    initialCaracter[2] = robot2InitialCharacter;
    finalCaracter[0] = robot0FinalCharacter;
    finalCaracter[1] = robot1FinalCharacter;
    finalCaracter[2] = robot2FinalCharacter;

    receiving = false;
    robot_speed = "";

    last_time = 0;
    status = false;
    openStatus = false;
}

Transmission::~Transmission(){
    closeConnection();
}

void Transmission::closeConnection(){
    close(usb);   
}

bool Transmission::openConection(){   

    //usb = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    usb = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_SYNC);

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
        cfsetospeed(&options, (speed_t)B9600);
        cfsetispeed(&options, (speed_t)B9600);

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
string Transmission::generateMessage(int robot, Command comand){

    short int start_delimiter = 0x7E;
    short int frame_type = 0x01;
    short int frame_id = 0x01;
    short int option = 0x00;

    short int address = 0; // ALTERAR PARA ROBOT
    int lenght = 0xC;

    vector<int> hex_message = comand.to_hex();

    int checksum = [&](){
        char check = frame_type + frame_id + address + option;
        unsigned char sum = 0;

        for(int i=0 ; i<hex_message.size() ; i++){
            sum = sum + (hex_message[i]); // soma decimal: 405
        }

        int aux = check+sum;
        std::string binary = std::bitset<8>(aux).to_string();	// Pega os 8 primeiros bits do valor
	    unsigned long decimal = std::bitset<8>(binary).to_ulong();	// Volta para decimal
        int checksum = 0xFF-decimal;

        return checksum;
    }();

    // retorna a string pronta para ser enviada
    stringstream ss;
    ss << std::hex << start_delimiter;
    ss << std::hex << 0x0 << 0x0 << 0x0 << lenght;
    ss << std::hex << 0x0 << frame_type;
    ss << std::hex << 0x0 << frame_id;
    ss << std::hex << 0x0 << 0x0 << 0x0 << address;
    ss << std::hex << 0x0 << option;

    for(int i=0 ; i<hex_message.size() ; i++){
        ss << std::hex << hex_message[i];
    }
    ss << std::hex << checksum;

//    cout << ss.str() << endl;
    return ss.str();
}

// recebe uma string para enviar
void Transmission::transmitting(string comand){
    const int size = comand.size();
    unsigned char send_bytes[size/2];

    if(timer.getTime() - last_time > 2000){
        string out = executeCommand("ls /dev/ttyUSB0 2> /dev/null");

        if(out.compare("") == 0) status = false;
        else status = true;

        last_time = timer.getTime();
    } 

    if(status == false || openStatus == false){
        closeConnection();
        openConection();

    } else {
        cout << "COMANDO: ";
        for(int i=0, cont=0 ; i<comand.size() ; i++){
            stringstream parcial;
            parcial << "0x" << comand[i];
            cout << comand[i];
            i++;
            cout << comand[i] << " ";
            parcial << comand[i];
            send_bytes[cont] = stoi(parcial.str().c_str(), 0, 16);
            cont++;
        }
        cout << endl;

        for(int i=0 ; i<size/2 ; i++){
            cout << int(send_bytes[i]) << " ";
        }
        cout << endl;
        //unsigned char bytestosend[] = { 0x7E , 0x00 , 0x0A , 0x01 , 0x01 , 0x00 , 0x00 , 0x00 , 0x54 , 0x65 , 0x73 , 0x74 , 0x65 , 0xF8};

        write(usb, send_bytes, sizeof(send_bytes));
    }
}

bool Transmission::getConnectionStatus(){
    return status && openStatus;
}

void Transmission::setMovements(vector<Command> mov){
    swap(movements, mov); // movements = mov;
}