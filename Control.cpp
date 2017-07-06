#include "Control.h"

Control::Control(){
	program_state = GAME;
	play = false;
	change_time = true;
}

void Control::handle(){

    vision.initialize();

	Strategy::initStaticParameters(); // precisa ser primeiro

	strategies.push_back(new StrategyAttack());
//	strategies.push_back(new StrategyDefense());
//	strategies.push_back(new StrategyGoal());/

	for(auto s: strategies){
		s->initialize();
	}

    while(getProgramState() == GAME){

    	timer.startTime();		
      
			vision.computerVision();
		
			Strategy::setObjects(vision.getTeam(), vision.getOpponent(), vision.getBall());
			Strategy::defineFunctions();

			for(int i=0 ; i<strategies.size() ; i++){
				strategies[i]->apply();
			}

			if(isPlaying()){
				for(int i=0 ; i<strategies.size() ; i++){
					transmission.send(strategies[i]->getRobotId(), strategies[i]->getCommand());
				}
			}

			
		timer.framesPerSecond();
		timer.waitTimeStarted(33);
    }
	
	vision.cameraRelease();
	transmission.stopAllRobots(3);
	for(auto s: strategies){
		delete s;
	}
}


int Control::GUI() {

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

	builder->get_widget("Box", box);
    box->pack_start(draw_robot);

	sigc::connection robot_draw_connection = Glib::signal_timeout().connect(sigc::mem_fun(this, &Control::setInformations50MilliSec), 50); 

    window->show_all();

    std::thread control_thread(&Control::handle, this);	

	app->run(*window);

	robot_draw_connection.disconnect();

	setProgramState(MENU);
	control_thread.join();
	
	return program_state;
}


bool Control::onKeyboard(GdkEventKey* event){

	if(event->type == GDK_KEY_RELEASE){
		for(int i=0 ; i<3 ; i++) transmission.send(i, Command(STOPPED_MOVE, 0, 0));
	} else if (event->keyval == GDK_KEY_Left) {
		for(int i=0 ; i<3 ; i++) transmission.send(i, Command(LEFT_MOVE, 150, 150));

    } else if (event->keyval == GDK_KEY_Right) {
		for(int i=0 ; i<3 ; i++) transmission.send(i, Command(RIGHT_MOVE, 150, 150));
	
	} else if (event->keyval == GDK_KEY_Up) {
		for(int i=0 ; i<3 ; i++) transmission.send(i, Command(FORWARD_MOVE, 150, 150));

	} else if (event->keyval == GDK_KEY_Down) {
		for(int i=0 ; i<3 ; i++) transmission.send(i, Command(BACK_MOVE, 150, 150));

	} else if(event->keyval == GDK_KEY_space) {
		button_play->set_active(!button_play->get_active());
		
	} else if(event->keyval == GDK_KEY_Escape){
		onMenuQuit();
	
	} else {
		for(int i=0 ; i<3 ; i++) transmission.send(i, Command(STOPPED_MOVE, 0, 0));
	}

    return true;
}

void Control::onPotencyChanged(){
//	strategy.setPowerPotency(spin_potency->get_value());
}

void Control::onCurveChanged(){
	//strategy.setPowerCurve(spin_curve->get_value());
}

void Control::onButtonPlay(){

	if(button_play->get_active()){
		button_play->set_label("Playing");
	} else {
		button_play->set_label("Paused");
	}

	setPlay(!isPlaying());
//	transmission.stopRobot();
}

void Control::onButtonTime(){

	if(getChangeTime() == false){
		button_side->set_label("1º time");
	} else {
		button_side->set_label("2º time");
	}

	/*
	Quando inverter:
	p2.x = size.x - p2.x
	p2.y = size.y - p2.y

	p1.x = size.x - p1.x
	p1.y = size.y - p1.y
*/

	setChangeTime(!getChangeTime());
}

bool Control::setInformations50MilliSec(){
	
	draw_robot.setPosition(vision.getPositions());

	if(transmission.getConnectionStatus() == false){
		label_transmission->set_visible(true);
	} else {
		label_transmission->set_visible(false);
	}
	
	label_fps->set_label("Fps: " + to_string(timer.getFps()));
	
	return true;
}

void Control::onMenuCalibration(){
    setProgramState(CALIBRATION); window->close();
}

void Control::onMenuSimulator(){
    setProgramState(SIMULATOR); window->close();
}

void Control::onMenuArduino(){
    setProgramState(ARDUINO); window->close();
}

void Control::onMenuQuit(){
    setProgramState(MENU); window->close();
}

void Control::setProgramState(int i){
    std::lock_guard<std::mutex> lock(mutex);        
    program_state = i;
}

int Control::getProgramState(){
    std::lock_guard<std::mutex> lock(mutex);        
    return program_state;
}

void Control::setChangeTime(bool b){
	std::lock_guard<std::mutex> lock(mutex); 
	change_time = b;
}

bool Control::getChangeTime(){
	std::lock_guard<std::mutex> lock(mutex); 
	return change_time;
}

void Control::setPlay(bool b){
	std::lock_guard<std::mutex> lock(mutex); 
	play = b;
}

bool Control::isPlaying(){
	std::lock_guard<std::mutex> lock(mutex); 
	return play;
}



