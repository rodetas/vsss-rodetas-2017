#include "Fps.h"

Fps::Fps(){    
    fpsStart=0;
    fpsAvg=0;
    fps1seg=0;
}

float Fps::getTime(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,  &t);
    return (t.tv_nsec*1e-6/1000); //milissgundos
}

float Fps::startFps(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,  &t);
    return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

float Fps::framesPerSecond(){
    if(startFps()-fpsStart > 1000){
        fpsStart=startFps();
        fpsAvg=0.7*fpsAvg+0.3*fps1seg;
        fps1seg=0;
    }
    fps1seg++;
    return int(fpsAvg);
}