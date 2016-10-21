#include "Control.h"
#include <thread>
#include <gtkmm.h>

Control::Control(){	
	objects.resize(7);
	movements.resize(3);
	manipulation.loadCalibration();
	program_state = MENU;
}

void Control::handle(){

	calibration.setGUICalibration(&calibration);

	std::thread control_thread([&] {program_state = menuGUI();} );

	while(program_state != EXIT){
		
		cout << "MENU" << endl; // se tirar o cout trava pq?

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
						program_state = EXIT;
					}
				}
				program_state = MENU; // retornar estado do programa atraves do game
				break;

			case SIMULATOR:
				simulator.openWindow();
				while(!simulator.getEndSimulator()){
					simulator.simulate();
					strategy.setObjects(simulator.getPositions());
					strategy.handleStrategies();
					simulator.setTargets(strategy.getTargets());
				}
				simulator.setEndSimulator(false);
				program_state = MENU; // retornar estado do programa atraves do simulator
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
			
			case EXIT:
				program_state = transmission.closeTransmission();
				break;
		}
	}

	control_thread.detach();
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

int Control::menuGUI(){

	auto app = Gtk::Application::create();

	Gtk::Window window;
	Gtk::Grid main_grid;
	Gtk::Button button_play;
	Gtk::Button button_calibration;
	Gtk::Button button_simulator;
	Gtk::Button button_arduino;
	Gtk::Button button_exit;

	window.set_default_size(400, 400);

	button_play.add_label("Play");
	button_calibration.add_label("Calibration");
	button_simulator.add_label("Simulator");
	button_arduino.add_label("Arduino");
	button_exit.add_label("Exit");

	Gtk::VBox mainLayout;

	mainLayout.pack_start(button_play, Gtk::PACK_EXPAND_WIDGET);
	mainLayout.pack_start(button_calibration, Gtk::PACK_EXPAND_WIDGET);
	mainLayout.pack_start(button_simulator, Gtk::PACK_EXPAND_WIDGET);
	mainLayout.pack_start(button_arduino, Gtk::PACK_EXPAND_WIDGET);
	mainLayout.pack_start(button_exit, Gtk::PACK_EXPAND_WIDGET);

	button_play.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonPlay) );
	button_calibration.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonCalibration) );
	button_simulator.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonSimulator) );
	button_arduino.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonArduino) );
	button_exit.signal_clicked().connect( sigc::mem_fun(this, &Control::onButtonExit) );

	mainLayout.show_all();
	window.add(mainLayout);

  	app->run(window);

	return EXIT;
}

void Control::onButtonPlay(){ program_state = GAME;}
void Control::onButtonCalibration(){ program_state = CALIBRATION; }
void Control::onButtonSimulator(){ program_state = SIMULATOR; }
void Control::onButtonArduino(){ program_state = ARDUINO; }
void Control::onButtonExit(){ program_state = EXIT; }