#ifndef CONTROL_H_
#define CONTROL_H_

#include "Header.h"
#include "Vision.h"
#include "Strategy.h"
#include "utils/Timer.h"
#include "Transmission/Connected.h"
#include "GUI/Cairo/DrawAreaControl.h"

#include <thread>
#include <mutex>
#include <gtkmm.h>

class Control {

private:
	Connected transmission;
	Strategy strategy;
	Vision vision;
	Timer timer;

	//GTKMM
	Glib::RefPtr<Gtk::Application> app;
	DrawAreaControl draw_robot;
	Gtk::Window window;
	Gtk::ToggleButton button_play;
	Gtk::Label label_fps;
	Gtk::Label label_transmission;

	int program_state;
	bool change_time;
	bool play;

	mutable std::mutex mutex;
	int thread_fps;
	bool thread_transmission_status;
	vector<rodetas::Object> thread_position;
	
	void GUIInformation();

	void onButtonPlay(Gtk::ToggleButton*);
	void onPotencyChanged(Gtk::SpinButton*);
	void onCurveChanged(Gtk::SpinButton*);
	void onButtonTime(Gtk::Button*);
	
	bool onKeyboard(GdkEventKey*);
	bool setInformations50MilliSec();
	void setThreadVariables();

	void onMenuCalibration();
	void onMenuSimulator();
	void onMenuArduino();
	void onMenuQuit();

public:
	Control();
	int handle();
};

#endif