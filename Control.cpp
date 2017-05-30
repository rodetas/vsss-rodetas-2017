#include "Control.h"

Control::Control(){
	program_state = GAME;
	play = false;
	change_time = true;
}

int Control::handle(){

    std::thread menu_thread(&Control::GUIInformation, this);

    // initialize classes
    vision.initialize();
    strategy.initialize();

    while(program_state == GAME){
		
		// recognize robot's points
		vision.computerVision();
		// apply strategies
	
		strategy.setObjects(vision.getPositions());
		strategy.handleStrategies();

		if (play){
			transmission.setMovements(strategy.getMovements());
			transmission.send();
		}
		//transmission.reading();
		transmission.transmitting("TESTE");
	
		timer.framesPerSecond();

		setThreadVariables();
		timer.waitTimeStarted(33);
    }
	
	vision.cameraRelease();

    menu_thread.detach();
	
	return program_state;
}

void Control::setThreadVariables(){
	{
		std::lock_guard<std::mutex> lock(mutex);
		thread_position = vision.getPositions();
		thread_fps = timer.getFps();
		thread_transmission_status = transmission.getConnectionStatus();
	}
}

void Control::GUIInformation() {

	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("GUI/Glade/Control.glade");

	builder->get_widget("Window Control", window);
   	window->signal_key_press_event().connect(sigc::mem_fun(this, &Control::onKeyboard), false);
	window->signal_key_release_event().connect(sigc::mem_fun(this, &Control::onKeyboard), false);
	window->maximize();

///////////////////////// BUTTONS /////////////////////////

	builder->get_widget("Button Paused", button_play); 
	button_play->signal_clicked().connect(sigc::mem_fun(this, &Control::onButtonPlay));

	builder->get_widget("Button Penalty", button_penalty); 
	//button_penalty->signal_clicked().connect(sigc::mem_fun(this, &Control::));

	builder->get_widget("Button Time", button_side);
	button_side->signal_clicked().connect(sigc::mem_fun(this, &Control::onButtonTime));

	builder->get_widget("Selector Potency", spin_potency);
	spin_potency->signal_value_changed().connect(sigc::mem_fun(this, &Control::onPotencyChanged));

	builder->get_widget("Selector Curve", spin_curve);
	spin_curve->signal_value_changed().connect(sigc::mem_fun(this, &Control::onCurveChanged));

	builder->get_widget("Label FPS", label_fps);

	builder->get_widget("Transmission Erro", label_transmission);

	///////////////////////// NAVEGATION MENU /////////////////////////

	builder->get_widget("Menu Calibrate", menu_calibration);
	menu_calibration->signal_activate().connect(sigc::mem_fun(this, &Control::onMenuCalibration));

	builder->get_widget("Menu Simulate", menu_simulator);
	menu_simulator->signal_activate().connect(sigc::mem_fun(this, &Control::onMenuSimulator));

	builder->get_widget("Menu Upload Arduino", menu_arduino);
	menu_arduino->signal_activate().connect(sigc::mem_fun(this, &Control::onMenuArduino));

	builder->get_widget("Menu Quit", menu_quit);
	menu_quit->signal_activate().connect(sigc::mem_fun(this, &Control::onMenuQuit));






	





    window->show_all();

	app->run(*window);
	
	/*

///////////////////////// DRAW IMAGE /////////////////////////

	sigc::connection robot_draw_connection = Glib::signal_timeout().connect(sigc::mem_fun(this, &Control::setInformations50MilliSec), 50); 


///////////////////////// CONTAINERS /////////////////////////
		
	Gtk::Grid box_right;
		//box_right.set_layout(Gtk::BUTTONBOX_CENTER );
		box_right.set_row_spacing(20);
		box_right.set_border_width(20);
		box_right.set_valign(Gtk::ALIGN_CENTER);

		box_right.attach(button_play,0,0,1,1);
		box_right.attach(button_penalty,0,1,1,1);
		box_right.attach(button_side,0,2,1,1);
		box_right.attach(box_potency,0,3,1,1);
		box_right.attach(box_curve,0,4,1,1);
		box_right.attach(label_fps,0,5,1,1);
		box_right.attach(label_transmission,0,6,1,3);

	Gtk::Box box_center(Gtk::ORIENTATION_VERTICAL);
		box_center.set_border_width(20);
		box_center.pack_start(draw_robot);

	Gtk::Box box_top(Gtk::ORIENTATION_HORIZONTAL);
		box_top.set_border_width(0);
        box_top.pack_start(menu_bar, Gtk::PACK_SHRINK);

	Gtk::Box box(Gtk::ORIENTATION_HORIZONTAL);
		box.pack_start(box_center);
		box.pack_start(box_right, false, false, 20);

	Gtk::Box global_box(Gtk::ORIENTATION_VERTICAL);
		global_box.pack_start(box_top, false, false, 0);
		global_box.pack_start(box);

	window.add(global_box);
	window.show_all();
	
  	app->run(window);

	program_state = MENU;
	robot_draw_connection.disconnect();*/
}


bool Control::onKeyboard(GdkEventKey* event){

	if(event->type == GDK_KEY_RELEASE){
		transmission.movementRobot(Command('P', 0, 0));
	} else if (event->keyval == GDK_KEY_Left) {
		transmission.movementRobot(Command('E', 150, 150));

    } else if (event->keyval == GDK_KEY_Right) {
		transmission.movementRobot(Command('D', 150, 150));
	
	} else if (event->keyval == GDK_KEY_Up) {
		transmission.movementRobot(Command('A', 150, 150));

	} else if (event->keyval == GDK_KEY_Down) {
		transmission.movementRobot(Command('V', 150, 150));

	} else if(event->keyval == GDK_KEY_space) {
		button_play->set_active(!button_play->get_active());
		
	} else if(event->keyval == GDK_KEY_Escape){
		onMenuQuit();
	
	} else {
		transmission.movementRobot(Command('P', 0, 0));
	}

    return true;
}

void Control::onPotencyChanged(){
	strategy.setPowerPotency(spin_potency->get_value());
}

void Control::onCurveChanged(){
	strategy.setPowerCurve(spin_curve->get_value());
}

void Control::onButtonPlay(){

	if(button_play->get_active()){
		button_play->set_label("Playing");
	} else {
		button_play->set_label("Paused");
	}

    play = !play;
	transmission.stopRobot();
}

void Control::onButtonTime(){

	if(change_time == false){
		button_side->set_label("1º time");
	} else {
		button_side->set_label("2º time");
	}

	change_time = !change_time;
}

bool Control::setInformations50MilliSec(){
	
	draw_robot.setPosition(thread_position);

	if(thread_transmission_status == false){
		label_transmission->set_visible(true);
	} else {
		label_transmission->set_visible(false);
	}
	
	label_fps->set_label("Fps: " + to_string(thread_fps));
	
	return true;
}

void Control::onMenuCalibration(){
    program_state = CALIBRATION; app->quit();
}

void Control::onMenuSimulator(){
    program_state = SIMULATOR; app->quit();
}

void Control::onMenuArduino(){
    program_state = ARDUINO; app->quit();
}

void Control::onMenuQuit(){
    program_state = MENU; app->quit();
}