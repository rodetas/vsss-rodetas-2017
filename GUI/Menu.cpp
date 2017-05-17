#include "Menu.h"

Menu::Menu(){
}
	
int Menu::GUI(){

	program_state = EXIT;
			
	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("GUI/Menu.glade");

	Gtk::Window* window = nullptr;
	Gtk::Button* button_play = nullptr;
	Gtk::Button* button_calibration = nullptr;
	Gtk::Button* button_simulator = nullptr;
	Gtk::Button* button_arduino = nullptr;
	Gtk::Button* button_exit = nullptr;
	
	builder->get_widget("Window Menu", window);
	builder->get_widget("Button Play", button_play);
	builder->get_widget("Button Calibration", button_calibration);
	builder->get_widget("Button Simulator", button_simulator);
	builder->get_widget("Button Arduino", button_arduino);
	builder->get_widget("Button Exit", button_exit);

	button_play->signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonPlay) );
	button_calibration->signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonCalibration) );
	button_simulator->signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonSimulator) );
	button_arduino->signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonArduino) );
	button_exit->signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonExit) );
	
	app->run(*window);

	delete window;
	delete button_play;
	delete button_calibration;
	delete button_simulator;
	delete button_arduino;
	delete button_exit;	

	return program_state;
} 

void Menu::onButtonPlay(){ 
	program_state = GAME; app->quit(); 
}
void Menu::onButtonCalibration(){ 
	program_state = CALIBRATION; app->quit(); 
}
void Menu::onButtonSimulator(){ 
	program_state = SIMULATOR; app->quit(); 
}
void Menu::onButtonArduino(){ 
	program_state = ARDUINO; app->quit(); 
}
void Menu::onButtonExit(){
	program_state = EXIT; app->quit();
}