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
		
//		timer.startTime();
		
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
		timer.framesPerSecond();

		setThreadVariables();
//		timer.waitTimeStarted(33);
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

	Glib::RefPtr<Gtk::AccelGroup> accel_map = Gtk::AccelGroup::create();

		window.maximize();
		window.set_title("Rodetas");
		window.set_icon_from_file("files/images/logo-rodetas.png");
		window.signal_key_press_event().connect(sigc::mem_fun(this, &Control::onKeyboard), false);
		window.signal_key_release_event().connect(sigc::mem_fun(this, &Control::onKeyboard), false);
		window.add_accel_group(accel_map);

///////////////////////// DRAW IMAGE /////////////////////////

	sigc::connection robot_draw_connection = Glib::signal_timeout().connect(sigc::mem_fun(this, &Control::setInformations50MilliSec), 50); 


///////////////////////// BUTTONS /////////////////////////

	button_play.add_label("Paused");	
	button_play.signal_clicked().connect( sigc::bind<Gtk::ToggleButton*> (sigc::mem_fun(this, &Control::onButtonPlay), &button_play) );

	Gtk::Button button_penalty;
		button_penalty.add_label("Penalty");

	Gtk::Button button_side;
		button_side.add_label("1º time");
		button_side.signal_clicked().connect( sigc::bind<Gtk::Button*> (sigc::mem_fun(this, &Control::onButtonTime), &button_side) );

	Gtk::Box box_potency(Gtk::ORIENTATION_VERTICAL);
		box_potency.set_spacing(10);

	Gtk::Label label_potency;
		label_potency.set_text("Potency Factor:");
		box_potency.pack_start(label_potency);

	Gtk::SpinButton spin_potency;
		spin_potency.set_range(0,3);
		spin_potency.set_value(1.2);
		spin_potency.set_digits(1);
		spin_potency.set_increments(0.1,0.1);
		spin_potency.signal_value_changed().connect( sigc::bind<Gtk::SpinButton*> (sigc::mem_fun(this, &Control::onPotencyChanged), &spin_potency) );
		box_potency.pack_start(spin_potency);

	Gtk::Box box_curve(Gtk::ORIENTATION_VERTICAL);
		box_curve.set_spacing(10);

	Gtk::Label label_curve;
		label_curve.set_text("Curve Factor:");
		label_curve.set_size_request(5,5);
		box_curve.pack_start(label_curve);

	Gtk::SpinButton spin_curve;
		spin_curve.set_range(0,3);
		spin_curve.set_value(1.1);
		spin_curve.set_digits(1);
		spin_curve.set_increments(0.1,0.3);
		spin_curve.signal_value_changed().connect( sigc::bind<Gtk::SpinButton*> (sigc::mem_fun(this, &Control::onCurveChanged), &spin_curve) );
		box_curve.pack_start(spin_curve);

	label_fps.set_label("Fps: 0");

	label_transmission.set_label("TRANSMISSION ERROR");
	label_transmission.override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);


///////////////////////// NAVEGATION MENU /////////////////////////

    Gtk::MenuItem menu_calibration;
        menu_calibration.set_label("_Calibrate");
        menu_calibration.set_use_underline(true);
        menu_calibration.add_accelerator("activate", accel_map, GDK_KEY_c, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
        menu_calibration.signal_activate().connect(sigc::mem_fun(this, &Control::onMenuCalibration));

    Gtk::MenuItem menu_simulator;
        menu_simulator.set_label("S_imulate");
        menu_simulator.set_use_underline(true);
        menu_simulator.add_accelerator("activate", accel_map, GDK_KEY_s, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
        menu_simulator.signal_activate().connect(sigc::mem_fun(this, &Control::onMenuSimulator));

    Gtk::MenuItem menu_arduino;
        menu_arduino.set_label("_Upload Arduino");
        menu_arduino.set_use_underline(true);
        menu_arduino.add_accelerator("activate", accel_map, GDK_KEY_u, Gdk::CONTROL_MASK, Gtk::ACCEL_VISIBLE);
        menu_arduino.signal_activate().connect(sigc::mem_fun(this, &Control::onMenuArduino));

    Gtk::MenuItem menu_quit;
        menu_quit.set_label("_Quit");
        menu_quit.set_use_underline(true);
        menu_quit.add_accelerator("activate", accel_map, GDK_KEY_Escape, Gdk::ModifierType(0), Gtk::ACCEL_VISIBLE);
        menu_quit.signal_activate().connect(sigc::mem_fun(this, &Control::onMenuQuit));

    Gtk::SeparatorMenuItem separator;    

    Gtk::MenuItem menu_navegation;
    Gtk::Menu subMenuNavigation;    
        menu_navegation.set_label("Navegation");
        menu_navegation.set_submenu(subMenuNavigation);
        subMenuNavigation.append(menu_calibration);
        subMenuNavigation.append(menu_simulator);
        subMenuNavigation.append(menu_arduino);
        subMenuNavigation.append(separator);
        subMenuNavigation.append(menu_quit);

	Gtk::MenuBar menu_bar;
        menu_bar.append(menu_navegation);
   
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

	robot_draw_connection.disconnect();
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
		button_play.set_active(!button_play.get_active());
		
	} else if(event->keyval == GDK_KEY_Escape){
		onMenuQuit();
	
	} else {
		transmission.movementRobot(Command('P', 0, 0));
	}

    return true;
}

void Control::onPotencyChanged(Gtk::SpinButton* button){
	strategy.setPowerPotency(button->get_value());
}

void Control::onCurveChanged(Gtk::SpinButton* button){
	strategy.setPowerCurve(button->get_value());
}

void Control::onButtonPlay(Gtk::ToggleButton* button){

	if(button->get_active()){
		button->set_label("Playing");
	} else {
		button->set_label("Paused");
	}

    play = !play;
	transmission.stopRobot();
}

void Control::onButtonTime(Gtk::Button* button){

	if(change_time == false){
		button->set_label("1º time");
	} else {
		button->set_label("2º time");
	}

	change_time = !change_time;
}

bool Control::setInformations50MilliSec(){
	
	draw_robot.setPosition(thread_position);

	if(thread_transmission_status == false){
		label_transmission.set_visible(true);
	} else {
		label_transmission.set_visible(false);
	}
	
	label_fps.set_label("Fps: " + to_string(thread_fps));
	
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