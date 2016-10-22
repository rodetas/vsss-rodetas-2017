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
#include "utils/Test.h"
#include "utils/Fps.h"
#include "CRUD/Manipulation.h"
#include <thread>


class Control{

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

	vector<Object> objects;
	vector<string> movements;
	
	int  program_state;
	bool program_run;

	int menuGUI();
	void onButtonPlay();
	void onButtonCalibration();
	void onButtonSimulator();
	void onButtonArduino();
	void onButtonExit();

public:
	Control();
	void setInformations();	
	void handle();	
};

#endif