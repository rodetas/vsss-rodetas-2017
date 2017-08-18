#include "Strategy.h"

Strategy* Strategy::instance = NULL;

Strategy::Strategy(){

}

void Strategy::apply(){
    
}

Strategy* Strategy::getInstance(){

    if(instance == NULL){
        instance = new Strategy();
    }

    return instance;
}