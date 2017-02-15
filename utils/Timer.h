#ifndef FPS_H_
#define FPS_H_

#include <iostream>
#include <unistd.h>

class Timer{
private:
    float fps;
    float cont;
    float last_time;
    float start_time;

public:
	Timer();
	float getTime();
    float framesPerSecond();
    float endTime();
    void startTime();
    void waitTime(float);
};
#endif