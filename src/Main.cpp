#include "game/Control.h"
#include "calibration/Calibration.h"
#include "simulator/Test.h"
#include "menu/Menu.h"
#include "arduino/Arduino.h"

int main(int argc, char *argv[]) {

	int program_state = MENU;

	while(program_state != EXIT){
		
		switch(program_state){

			case GAME:{
				Control control;
				program_state = control.GUI();
			} break;
			
			case CALIBRATION:{
				Calibration calibration;
				program_state = calibration.GUI();
			} break;
			
			case MENU:{
				Menu menu;
				program_state = menu.GUI();
			} break;

			case ARDUINO:{
				Arduino arduino;
				program_state = arduino.GUI();
			} break;

			case SIMULATOR:{
				Test test;
				program_state = test.GUI();
			} break;
			
			case EXIT:{
				
			} break;
		}
	}

	return 0;
}