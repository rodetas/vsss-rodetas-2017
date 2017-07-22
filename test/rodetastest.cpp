#include <iostream>
#include <assert.h>

#include "../src/game/Transmission.h"

using namespace std;

void unitTest_TransmissionGenerateMessage(){

    int id = 0;
    Command c(FORWARD_MOVE, 255, 255);
    Transmission t;

    string expected = t.generateMessage(id, c);
    assert(expected.compare("7e000c0101000000463235353235357f") == 0);

    id = 1;
    Command c1(BACK_MOVE, 123, 123);
    expected = t.generateMessage(id, c1);
    assert(expected.compare("7e000c0101000100423132333132338e") == 0);
}

void unitTest_TransmissionGenerateCheckSum(){

    int frame = 0x01;
    int id = 0x01;
    int address = 0;
    int option = 0;
    vector<int> hex = {0x42, 0x31, 0x32, 0x33, 0x31, 0x32, 0x33}; //B123123

    Transmission t;
    int expected = t.generateCheckSum(frame, id, address, option, hex);

    assert(expected == 0x8F);

    hex = {0x4C, 0x32, 0x35, 0x35, 0x32, 0x35, 0x35};
    expected = t.generateCheckSum(frame, id, address, option, hex); //L255255

    assert(expected == 0x79);

}


int main(int argc, char** argv){
    
    // Transmissao
    unitTest_TransmissionGenerateMessage();
    unitTest_TransmissionGenerateCheckSum();


    return 0;
}