#ifndef CONTROL_H_
#define CONTROL_H_

#include "Header.h"
#include "Vision.h"
#include "Calibration.h"
#include "Transmission/Transmission.h"
#include "Transmission/ConectadoJogo.h"
#include "Transmission/Desconectado.h"

#include "utils/Timer.h"

#include "Strategy.h"
#include "CRUD/Manipulation.h"

#include "GUI/Cairo/DrawAreaControl.h"

#include <thread>
#include <gtkmm.h>

class Control {

private:

	Manipulation manipulation;
	Transmission* transmission;
	Strategy strategy;
	Vision vision;

	//GTKMM
	DrawAreaControl draw_robot;

	vector<rodetas::Object> objects;
	vector<string> movements;

	int program_state;
	
	void GUIInformation();
	bool onKeyboard(GdkEventKey*);

	
public:
	Control();
	int handle();
	bool sendPosition();
	void setInformations();

};

#endif