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
	
	Gtk::Window *window = nullptr;
	Gtk::ToggleButton *button_play = nullptr;
	Gtk::ToggleButton *button_penalty = nullptr;
	Gtk::ToggleButton *button_side = nullptr;
	Gtk::SpinButton *spin_potency = nullptr;
	Gtk::SpinButton *spin_curve  = nullptr;
	Gtk::Label *label_fps = nullptr;
	Gtk::Label *label_transmission = nullptr;
	Gtk::MenuItem *menu_calibration = nullptr;
	Gtk::MenuItem *menu_simulator = nullptr;
	Gtk::MenuItem *menu_arduino = nullptr;
	Gtk::MenuItem *menu_quit = nullptr;
	Gtk::SeparatorMenuItem *separator = nullptr; 


	int program_state;
	bool change_time;
	bool play;

	mutable std::mutex mutex;
	int thread_fps;
	bool thread_transmission_status;
	vector<rodetas::Object> thread_position;
	
	void GUIInformation();

	void onButtonPlay();
	void onPotencyChanged();
	void onCurveChanged();
	void onButtonTime();
	
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