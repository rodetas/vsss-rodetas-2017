#include "ArduinoControl.h"

ArduinoControl::ArduinoControl(){

}

ArduinoControl::~ArduinoControl(){

}

void ArduinoControl::control(){

    ArduinoView view;
    ArduinoModel model;

    std::thread view_thread(&ArduinoControl::threadView, this);
    std::thread model_thread(&ArduinoControl::threadModel, this);
    
    view_thread.join();
    model_thread.join();
    
}

void ArduinoControl::threadView(){

}

void ArduinoControl::threadModel(){
    
}

void notifyFromView(){
    
}