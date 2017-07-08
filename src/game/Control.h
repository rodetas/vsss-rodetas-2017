#ifndef CONTROL_H_
#define CONTROL_H_

#include "../Header.h"
#include "Vision.h"
#include "Strategies.h"
#include "../utils/Timer.h"
#include "Transmission.h"
#include "DrawAreaControl.h"

#include <thread>
#include <mutex>
#include <gtkmm.h>

class Control {

private:
	Transmission transmission;
	vector<StrategyFactory*> strategies;
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
	Gtk::Box *box = nullptr;

	std::mutex mutex;
	
	void onButtonPlay();
	void onPotencyChanged();
	void onCurveChanged();
	void onButtonTime();
	
	bool onKeyboard(GdkEventKey*);
	bool setInformations50MilliSec();

	void onMenuCalibration();
	void onMenuSimulator();
	void onMenuArduino();
	void onMenuQuit();

	int program_state;
	void setProgramState(int);
	int getProgramState();

	bool change_time;
	void setChangeTime(bool);
	bool getChangeTime();

	bool play;
	void setPlay(bool);
	bool isPlaying();

public:
	Control();
	int GUI();
	void handle();
};

#endif