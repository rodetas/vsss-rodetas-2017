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

    short int address = robot; // ALTERAR PARA ROBOT
    int lenght = 0xC;

    vector<int> hex_message = comand.to_hex();

    int checksum = [&](){
        char check = frame_type + frame_id + address + option;
        unsigned char sum = 0;

        for(int i=0 ; i<hex_message.size() ; i++){
            sum = sum + (hex_message[i]);
        }

        int checksum = check+sum;
        std::string binary = std::bitset<8>(checksum).to_string();	// Pega os 8 primeiros bits do valor
	    unsigned long decimal = std::bitset<8>(binary).to_ulong();	// Volta para decimal
        checksum = 0xFF-decimal;

        return checksum;
    }();

    // agrupa os parametros formando a mensagem pronta para ser enviada
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

// retorna a string pronta para ser enviada
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
        for(int i=0, cont=0 ; i<comand.size() ; i++){
            stringstream parcial;
            parcial << comand[i];
            i++;
            parcial << comand[i];
            send_bytes[cont] = stoi(parcial.str().c_str(), 0, 16);
            cont++;
        }
//        cout << send_bytes << endl;
        write(usb, send_bytes, sizeof(send_bytes));
    }
}

bool Transmission::getConnectionStatus(){
    std::lock_guard<std::mutex> lock(mutex);
    return status && openStatus;
}

void Transmission::setMovements(vector<Command> mov){
    std::lock_guard<std::mutex> lock(mutex);
    swap(movements, mov); // movements = mov;
}