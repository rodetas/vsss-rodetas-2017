#include "ArduinoControl.h"

ArduinoControl::ArduinoControl(){

}

ArduinoControl::~ArduinoControl(){

}

void ArduinoControl::control(){

    ArduinoView view;
    ArduinoModel model;

    //view.setModel(&model);

    std::thread* model_thread = new std::thread([&]{
        
    
    });

    std::thread view_thread(&ArduinoView::GUI, view);
    view_thread.join();
    
}