#ifndef MENU_H_
#define MENU_H_

#include "Header.h"
#include <gtkmm.h>

class Menu {

private:
	Glib::RefPtr<Gtk::Application> app;
	Gtk::Window* window = nullptr;
	Gtk::Button* button_play = nullptr;
	Gtk::Button* button_calibration = nullptr;
	Gtk::Button* button_simulator = nullptr;
	Gtk::Button* button_arduino = nullptr;
	Gtk::Button* button_exit = nullptr;
	
	int program_state;

	bool onKeyboard(GdkEventKey*);
	void onButtonPlay();
	void onButtonCalibration();
	void onButtonSimulator();
	void onButtonArduino();
	void onButtonExit();

public:
	Menu();
	~Menu();
	int GUI();    
};

#endif