#ifndef CONTROL_H_
#define CONTROL_H_

#include "Header.h"
#include "Vision.h"
#include "Strategy.h"
#include "utils/Timer.h"
#include "Transmission/Connected.h"
#include "GUI/Cairo/DrawAreaControl.h"

#include <thread>
#include <gtkmm.h>

class Control {

private:
	Connected transmission;
	Strategy strategy;
	Vision vision;
	Timer timer;

	//GTKMM
	DrawAreaControl draw_robot;
	Gtk::Window window;
	Glib::RefPtr<Gtk::Application> app;

	Gtk::ToggleButton button_play;
	Gtk::Label label_fps;
	Gtk::Label label_transmission;

	int program_state;
	bool changeTime;
	bool play;
	
	void GUIInformation();

	void onButtonPlay(Gtk::ToggleButton*);
	void onPotencyChanged(Gtk::SpinButton*);
	void onCurveChanged(Gtk::SpinButton*);
	void onButtonTime(Gtk::Button*);
	
	bool onKeyboard(GdkEventKey*);
	bool setInformations50MilliSec();

	void onMenuCalibration();
	void onMenuSimulator();
	void onMenuArduino();
	void onMenuQuit();

public:
	Control();
	int handle();
};

#endif