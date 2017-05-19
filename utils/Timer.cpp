#include "Timer.h"

Timer::Timer(){    
    fps = 0;
    cont = 0;
    last_time = 0;
    start_time = 0;
}

float Timer::getTime(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,  &t);
    return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

float Timer::framesPerSecond(){
    if(getTime() - last_time > 1000){
        last_time = getTime();
        fps = 0.7 * fps + 0.3 * cont;
        cont = 0;
    }
    cont++;
    return int(fps);
}

void Timer::startTime(){
    start_time = getTime();
}

float Timer::endTime(){
    return (getTime() - start_time);
}

void Timer::waitTimeStarted(float time_wait){
    while((getTime() - start_time) < time_wait){
        usleep(1000);
        start_time = getTime();
    }
}

void Timer::wait(int t){
    usleep(t);
}

int Timer::getFps(){
    return this->fps;
}