#include "Control.h"
#include <thread>
#include <gtkmm.h>

Control::Control(){	
	objects.resize(7);
	movements.resize(3);
	manipulation.loadCalibration();
	program_run = true;
	program_state = MENU;
}

void Control::handle(){

	calibration.setGUICalibration(&calibration);

	std::thread control_thread(&Control::menuGUI, this);

	while(program_run){
		cout << "MENU" << endl;
		switch(program_state){
			case GAME:
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
						program_run = false;
					}
				}
				program_state = MENU; // retornar estado do programa atraves do game
				break;
			
			case CALIBRATION:	
				program_state = calibration.calibrate();
				break;
			
			case ARDUINO:			
				program_state = arduino.loop();
				break;

			case TEST:
				program_state = test.loop();
				break;

			case SIMULATOR:					
				simulator.openWindow();
				while(!simulator.getEndSimulator()){
					simulator.simulate();
					strategy.setObjects(simulator.getPositions());
					strategy.handleStrategies();
					simulator.setTargets(strategy.getTargets());
					//simulator.setInformation(strategy.getInformation());
				}
				simulator.setEndSimulator(false);
				program_state = MENU; // retornar estado do programa atraves do simulator
				break;
			
			case EXIT:
				for(int i=0 ; i<3 ; i++) { transmission.sendMovement(i, STOPPED_MOVE,0); }
				transmission.closeConnection();
				program_run = false;
				break;
		}
	}
}

void Control::setInformations(){

	for (int i = 0; i < 3; i++){
		objects[i] = vision.getRobotTeam()[i];
		objects[i+3] = vision.getOpponent()[i];
	}
    objects[graphicBall] = vision.getBall();

	strategy.setObjects(objects);
	strategy.setPowerCurve(graphic.getPowerCurve());
	strategy.setPower(graphic.getPower());
	
	graphic.setObjects(objects);
	graphic.setTargets(strategy.getTargets());
	graphic.setFps(fps.framesPerSecond());
	graphic.setConnectionStatus(transmission.getConnectionStatus());
	graphic.setInformation(strategy.getInformation());

	vision.setRotateField(graphic.getRotateField());
}

void Control::menuGUI(){

	auto app = Gtk::Application::create();

	Gtk::Window window;
	Gtk::Grid main_grid;
	Gtk::Button button_play;
	Gtk::Button button_calibration;
	Gtk::Button button_simulator;
	Gtk::Button button_arduino;
	Gtk::Button button_exit;

	//window.set_default_size(200, 200);

	button_play.add_label("Play");
	button_calibration.add_label("Calibration");
	button_simulator.add_label("Simulator");
	button_arduino.add_label("Arduino");
	button_exit.add_label("Exit");

	main_grid.attach(button_play,0,0,1,1);
	main_grid.attach(button_calibration,0,1,1,1);
	main_grid.attach(button_simulator,0,2,1,1);
	main_grid.attach(button_arduino,0,3,1,1);
	main_grid.attach(button_exit,0,4,1,1);

	button_play.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonPlay) );
	button_calibration.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonCalibration) );
	button_simulator.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonSimulator) );
	button_arduino.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonArduino) );
	button_exit.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonExit) );

	main_grid.show_all();
	window.add(main_grid);

  	app->run(window);
}

void Control::onButtonPlay(){ program_state = GAME; }
void Control::onButtonCalibration(){ program_state = CALIBRATION; }
void Control::onButtonSimulator(){ program_state = SIMULATOR; }
void Control::onButtonArduino(){ program_state = ARDUINO; }
void Control::onButtonExit(){ program_state = EXIT; }