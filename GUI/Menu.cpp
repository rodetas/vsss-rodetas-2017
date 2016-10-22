#include "Menu.h"

Menu::Menu(){
}
	
int Menu::GUI(){
			
	app = Gtk::Application::create();

	Gtk::Window window;	
		window.set_default_size(400, 400);
	
	Gtk::Button button_play, button_calibration, button_simulator, button_arduino, button_exit;
		button_play.add_label("Play");
		button_calibration.add_label("Calibration");
		button_simulator.add_label("Simulator");
		button_arduino.add_label("Arduino");
		button_exit.add_label("Exit");

		button_play.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonPlay) );
		button_calibration.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonCalibration) );
		button_simulator.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonSimulator) );
		button_arduino.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonArduino) );
		button_exit.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonExit) );

	Gtk::VBox mainLayout;
		mainLayout.pack_start(button_play, Gtk::PACK_EXPAND_WIDGET);
		mainLayout.pack_start(button_calibration, Gtk::PACK_EXPAND_WIDGET);
		mainLayout.pack_start(button_simulator, Gtk::PACK_EXPAND_WIDGET);
		mainLayout.pack_start(button_arduino, Gtk::PACK_EXPAND_WIDGET);
		mainLayout.pack_start(button_exit, Gtk::PACK_EXPAND_WIDGET);
		mainLayout.show_all();

	window.add(mainLayout);

  	app->run(window);
	
	return program_state;
} 

void Menu::onButtonPlay(){ 
	program_state = GAME; 
	app->quit();	
}

void Menu::onButtonCalibration(){ program_state = CALIBRATION; }
void Menu::onButtonSimulator(){ program_state = SIMULATOR; }
void Menu::onButtonArduino(){ program_state = ARDUINO; }
void Menu::onButtonExit(){ program_state = EXIT; }