#ifndef CONTROL_H_
#define CONTROL_H_

#include "Header.h"
#include "Vision.h"
#include "Calibration.h"
#include "Transmission.h"
#include "Strategy.h"
#include "GUI/Simulator.h"
#include "GUI/Arduino.h"
#include "GUI/Menu.h"
#include "GUI/Graphic.h"
#include "GUI/Cairo_Robot.h"
#include "utils/Test.h"
#include "utils/Fps.h"
#include "CRUD/Manipulation.h"
#include <thread>
#include <gtkmm.h>

class Control {

private:
	Menu menu;
	Calibration calibration;
	Manipulation manipulation;
	Transmission transmission;
	Strategy strategy;
	Vision vision;
	Graphic graphic;
	Fps fps;
	Simulator simulator;
	Arduino arduino;
	Test test;

	bool game;

	vector<rod::Object> objects;
	vector<string> movements;
	
	int  program_state;
	bool program_run;

	Glib::RefPtr<Gtk::Application> app;
	Cairo_Robot* draw_robot2;
	
	int GUIInformation();
 
public:
	Control();
	void setInformations();	
	void handle();
};

#endif