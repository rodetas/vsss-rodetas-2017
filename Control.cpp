#include "Control.h"

Control::Control(){	
	objects.resize(7);
	movements.resize(3);
	manipulation.loadCalibration();
	program_state = MENU;
}

void Control::handle(){

	calibration.setGUICalibration(&calibration);

	while(program_state != EXIT){
		
		switch(program_state){
			case GAME:{

				std::thread menu_thread(bind(&Control::GUIInformation, this));
  
			// initialize classes
				vision.initialize();
				graphic.initialize();
				strategy.initialize(manipulation.getImageSize(), manipulation.getGoal());

				// game loop
				bool game = true;
				while(game){

				// recognize robot's points
					vision.makeVision(); 
				// set informations to other classes
					setInformations(); 
				// show information on screen
					graphic.gameInformation();
				// apply strategies
					strategy.handleStrategies(); 

					if(!graphic.isPause()){
						movements = strategy.getMovements();
						transmission.setMovements(movements);
					} else {
						for(int i=0 ; i<3 ; i++){
							transmission.sendMovement(i, graphic.getTestMovement(), 100100);
						}
					}

					if(graphic.getMenu()){
						vision.setCameraRelease();		
						graphic.setMenu(false);
						game = false;
					} else if(graphic.getClose()){
						vision.setCameraRelease();
						game = false;
						program_state = EXIT;
					}
				}
				menu_thread.detach();
				program_state = MENU; // retornar estado do programa atraves do game
			} break;

			case SIMULATOR:{
				simulator.openWindow();
				while(!simulator.getEndSimulator()){
					simulator.simulate();
					strategy.setObjects(simulator.getPositions());
					strategy.handleStrategies();
					simulator.setTargets(strategy.getTargets());
				}
				simulator.setEndSimulator(false);
				program_state = MENU; // retornar estado do programa atraves do simulator
			} break;
			
			case CALIBRATION:{
				program_state = calibration.calibrate();
			} break;
			
			case ARDUINO:{
				program_state = arduino.loop();
			} break;

			case TEST:{
				program_state = test.loop();
			} break;

			case MENU:{
				program_state = menu.GUI();
			} break;
			
			case EXIT:{
				program_state = transmission.closeTransmission();
			} break;
		}
	}
}

void Control::setInformations(){

	objects = vision.getPositions();

	strategy.setObjects(objects);
	strategy.setPowerCurve(graphic.getPowerCurve());
	strategy.setPower(graphic.getPower());
	
	graphic.setObjects(objects);
	graphic.setTargets(strategy.getTargets());
	graphic.setFps(fps.framesPerSecond());
	graphic.setConnectionStatus(transmission.getConnectionStatus());
	graphic.setInformation(strategy.getInformation());
}


int Control::GUIInformation() {
	Glib::RefPtr<Gtk::Application> app;
		app = Gtk::Application::create();

	Gtk::Window window;	
		window.maximize();
		window.set_title("Rodetas");
	
	Cairo_Robot draw_robot;	
		sigc::connection robot_draw_connection = Glib::signal_timeout().connect(sigc::bind< Cairo_Robot* > ( sigc::mem_fun(this, &Control::setRobot), &draw_robot) , 50 );

	Gtk::Button button_play, button_pause, button_side, button_penalty;
		button_play.add_label("Play");
		button_pause.add_label("Pause");
		button_side.add_label("Side");
		button_penalty.add_label("Penalty");

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
} 

bool Control::setRobot(Cairo_Robot *c){
	c->setPosition(objects);
	return true;
}