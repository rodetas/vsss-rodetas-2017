#include "Control.h"

Control::Control(){
	program_state = GAME;
	play = false;
}

int Control::handle(){

    std::thread menu_thread(bind(&Control::GUIInformation, this));

    // initialize classes
    vision.initialize();
    strategy.initialize();

    while(program_state == GAME){
		
		//timer.startTime();
		
		// recognize robot's points
		vision.computerVision();
		// set informations to other classes
		strategy.setObjects(vision.getPositions());
		// apply strategies
		strategy.handleStrategies();

		if (play){
			transmission.setMovements(strategy.getMovements());
			transmission.send();
		}
		//transmission.reading();

		//timer.waitTimeStarted(33);
		//cout << timer.framesPerSecond() << endl;
    }
	
	vision.cameraRelease();

    menu_thread.detach();

	return program_state;
}

void Control::GUIInformation() {
	
	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::AccelGroup> accel_map = Gtk::AccelGroup::create();

		window.maximize();
		window.set_title("Rodetas");
		window.set_icon_from_file("files/images/logo-rodetas.png");
		window.signal_key_press_event().connect(sigc::mem_fun(this, &Control::onKeyboard), false);
		window.add_accel_group(accel_map);

///////////////////////// DRAW IMAGE /////////////////////////

	sigc::connection robot_draw_connection = Glib::signal_timeout().connect(sigc::mem_fun(this, &Control::setPositionToDraw), 50); 


///////////////////////// BUTTONS /////////////////////////

	Gtk::Button button_pause, button_side, button_penalty;
		button_pause.add_label("Pause");
		button_side.add_label("Side");
		button_penalty.add_label("Penalty");
	
	Gtk::ToggleButton button_play;

		button_play.add_label("Play");		
		button_play.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonPlay) );


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

	Gtk::ButtonBox box_left(Gtk::ORIENTATION_VERTICAL);
		box_left.set_layout(Gtk::BUTTONBOX_CENTER );
		box_left.set_spacing(20);		 
		box_left.pack_start(button_play);
		box_left.pack_start(button_pause);		

	Gtk::ButtonBox box_right(Gtk::ORIENTATION_VERTICAL);
		box_right.set_layout(Gtk::BUTTONBOX_CENTER );
		box_right.set_spacing(20);
		box_right.pack_start(button_penalty);
		box_right.pack_start(button_side);
   
	Gtk::Box box_center(Gtk::ORIENTATION_VERTICAL);
		box_center.set_border_width(20);
		box_center.pack_start(draw_robot);

	Gtk::Box box_top(Gtk::ORIENTATION_HORIZONTAL);
		box_top.set_border_width(0);
        box_top.pack_start(menu_bar, Gtk::PACK_SHRINK);

	Gtk::Box box(Gtk::ORIENTATION_HORIZONTAL);
		box.pack_start(box_left, false, false, 20);
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
    if (event->keyval == GDK_KEY_Left) {
		transmission.movementRobot(Command('A', 100, 100));

    } else if (event->keyval == GDK_KEY_Right) {
		transmission.movementRobot(Command('A', 150, 150));
	
	} else if (event->keyval == GDK_KEY_Up) {
		transmission.movementRobot(Command('A', 200, 200));

	} else if (event->keyval == GDK_KEY_Down) {
		transmission.movementRobot(Command('A', 254, 254));

	} else if(event->keyval == GDK_KEY_space) {
		onButtonPlay();

	} else {
		transmission.movementRobot(Command('P', 0, 0));
	}

    return true;
}

void Control::onButtonPlay() {
    play = !play;
	transmission.stopRobot();
}

bool Control::setPositionToDraw(){
	draw_robot.setPosition(vision.getPositions());
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
