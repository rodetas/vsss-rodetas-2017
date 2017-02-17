#ifndef CONTROL_H_
#define CONTROL_H_

#include "Header.h"
#include "Vision.h"
#include "Strategy.h"
#include "utils/Timer.h"
#include "Transmission/ConectadoJogo.h"
#include "GUI/Cairo/DrawAreaControl.h"

#include <thread>
#include <gtkmm.h>

class Control {

private:
	ConectadoJogo transmission;
	Strategy strategy;
	Vision vision;
	Timer timer;

	//GTKMM
	DrawAreaControl draw_robot;

	int program_state;
	bool play;
	
	void GUIInformation();
	void onButtonPlay();
	bool onKeyboard(GdkEventKey*);
	bool setPositionToDraw();

public:
	Control();
	int handle();
};

#endif