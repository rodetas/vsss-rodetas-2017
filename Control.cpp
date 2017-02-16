#include "Control.h"

Control::Control(){	
	objects.resize(7);
	movements.resize(3);
	manipulation.loadCalibration();	
	//transmission = new Desconectado();
	program_state = GAME;
	play = false;
}

int Control::handle(){	

    std::thread menu_thread(bind(&Control::GUIInformation, this));

    // initialize classes
    vision.initialize();
    strategy.initialize(manipulation.getImageSize(), manipulation.getGoal());

	Timer timer;
    // game loop
    bool game = true;
    //transmission = new ConectadoJogo();
    while(program_state == GAME){
		
		timer.startTime();
		
		// recognize robot's points
		vision.makeVision();
		// set informations to other classes
		setInformations();
		// apply strategies
		strategy.handleStrategies();

		if (play){
			transmission.setMovements(strategy.getMovements());
			transmission.send();
		}
		transmission.reading();

		timer.waitTime(33);
    }

    //delete transmission;
    //transmission = new Desconectado();	

    menu_thread.detach();
			
	return program_state;
}

void Control::setInformations(){

	objects = vision.getPositions();

/*
	strategy.setObjects(objects);
	strategy.setPowerCurve(graphic.getPowerCurve());
	strategy.setPower(graphic.getPower());
*/
}

void Control::GUIInformation() {
	
	Glib::RefPtr<Gtk::Application> app;
		app = Gtk::Application::create();

	Gtk::Window window;	
		window.maximize();
		window.set_title("Rodetas");
		window.signal_key_press_event().connect(sigc::mem_fun(this, &Control::onKeyboard), false);
	
///////////////////////// DRAW IMAGE /////////////////////////

	sigc::connection robot_draw_connection = Glib::signal_timeout().connect(sigc::mem_fun(this, &Control::sendPosition), 50); 

  
///////////////////////// BUTTONS /////////////////////////

	Gtk::Button button_pause, button_side, button_penalty;
		button_pause.add_label("Pause");
		button_side.add_label("Side");
		button_penalty.add_label("Penalty");
	
	Gtk::ToggleButton button_play;

		button_play.add_label("Play");		
		button_play.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonPlay) );


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
		
	Gtk::Box box(Gtk::ORIENTATION_HORIZONTAL);
		box.pack_start(box_left, false, false, 20);
		box.pack_start(box_center);
		box.pack_start(box_right, false, false, 20);
		
	window.add(box);
	window.show_all();
	
  	app->run(window);

	robot_draw_connection.disconnect();

	program_state = MENU;
}

bool Control::sendPosition(){
	draw_robot.setPosition(objects);
	return true;
}

bool Control::onKeyboard(GdkEventKey* event){
    if (event->keyval == GDK_KEY_Left) {
		transmission.movementRobot(Command('E', 150, 150));

    } else if (event->keyval == GDK_KEY_Right) {
		transmission.movementRobot(Command('D', 150, 150));
	
	} else if (event->keyval == GDK_KEY_Up) {
		transmission.movementRobot(Command('A', 80, 80));

	} else if (event->keyval == GDK_KEY_Down) {
		transmission.movementRobot(Command('A', 250, 250));

	} else {
		transmission.movementRobot(Command('P', 0, 0));
	}

    return true;
}

void Control::onButtonPlay() {
    play = !play;
	transmission.stopRobot();
}