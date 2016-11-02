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
				game = true;
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

	app = Gtk::Application::create();

	Gtk::Window window;	
		window.maximize();
		window.set_title("Rodetas");

	Cairo_Robot draw_robot;
		Glib::signal_timeout().connect(sigc::bind<rod::Object>(sigc::mem_fun(draw_robot, &Cairo_Robot::setRobot), objects[0]), 50 );
				// Descobrir qual evento (signal_??) atualizaria o desenho, o método no Cairo_Robot é do tipo que o signal necessita
				// draw_robot.signal_draw().connect( sigc::mem_fun(draw_robot, &Cairo_Robot::setRobot) ); 

	window.add(draw_robot);
	window.show_all();

  	app->run(window);

	return program_state;
}