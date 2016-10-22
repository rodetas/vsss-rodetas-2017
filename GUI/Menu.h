#ifndef MENU_H_
#define MENU_H_

#include "../Header.h"
#include <gtkmm.h>

class Menu  {

private:
	Glib::RefPtr<Gtk::Application> app;
	int program_state;

	void onButtonPlay();
	void onButtonCalibration();
	void onButtonSimulator();
	void onButtonArduino();
	void onButtonExit();

public:
	Menu();	
	int GUI();    
};

#endif