#include "Menu.h"

Menu::Menu(){
}
	
int Menu::GUI(){

	program_state = EXIT;
			
	app = Gtk::Application::create();

	Gtk::Window window;	
		window.set_default_size(800, 600);
		window.set_position(Gtk::WIN_POS_CENTER_ALWAYS);
		window.set_title("Rodetas");

	Gtk::Button button_play, button_calibration, button_simulator, button_arduino, button_exit;
		button_play.add_label("Play");
		button_calibration.add_label("Calibration");
		button_simulator.add_label("Simulator");
		button_arduino.add_label("Arduino");
		button_exit.add_label("Exit");

		//button_play.set_size_request(200, 20);
		button_play.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonPlay) );
		button_calibration.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonCalibration) );
		button_simulator.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonSimulator) );
		button_arduino.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonArduino) );
		button_exit.signal_clicked().connect( sigc::mem_fun(this, &Menu::onButtonExit) );

	Gtk::ButtonBox button_box(Gtk::ORIENTATION_VERTICAL);
		button_box.set_layout(Gtk::BUTTONBOX_CENTER );
		button_box.set_spacing(20);
		button_box.pack_start(button_play);
		button_box.pack_start(button_calibration);
		button_box.pack_start(button_simulator);
		button_box.pack_start(button_arduino);
		button_box.pack_start(button_exit);
		button_box.show_all();

	window.add(button_box);

  	app->run(window);
	
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