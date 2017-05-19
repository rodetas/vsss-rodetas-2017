#include "Menu.h"

Menu::Menu(){
}

Menu::~Menu(){
}

int Menu::GUI(){

	program_state = EXIT;
			
	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("GUI/Glade/Menu.glade");
	
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

	return program_state;
} 

void Menu::onButtonPlay(){ 
	program_state = GAME; 
	window->close(); 
}
void Menu::onButtonCalibration(){ 
	program_state = CALIBRATION; 
	window->close(); 
}
void Menu::onButtonSimulator(){ 
	program_state = SIMULATOR; 
	window->close(); 
}
void Menu::onButtonArduino(){ 
	program_state = ARDUINO; 
	window->close(); 
}
void Menu::onButtonExit(){
	program_state = EXIT; 
	window->close();
}