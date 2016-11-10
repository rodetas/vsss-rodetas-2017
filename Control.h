#ifndef CONTROL_H_
#define CONTROL_H_

#include "Header.h"
#include "Vision.h"
#include "Calibration.h"
#include "Transmission.h"
#include "Strategy.h"
#include "GUI/Arduino.h"
#include "GUI/Menu.h"
#include "GUI/CairoDraw.h"
#include "utils/Fps.h"
#include "CRUD/Manipulation.h"
#include <thread>
#include <gtkmm.h>

class Control {

private:
	Menu menu;
	//Calibration calibration;
	Manipulation manipulation;
	Transmission transmission;
	Strategy strategy;
	Vision vision;
	Fps fps;

	vector<rodetas::Object> objects;
	vector<string> movements;
	
	int  program_state;
	
	void GUIInformation();
	bool setRobot(CairoDraw*);
	
public:
	Control();
	void setInformations();
	void handle();
};

#endif