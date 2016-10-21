#ifndef FPS_H_
#define FPS_H_

#include <iostream>

class Fps{
private:
    float fpsStart;
    float fpsAvg;
    float fps1seg;

public:
	Fps();
	float getTime();
	float startFps();
    float framesPerSecond();
};
#endif