#include "Control.h"
#include "Calibration.h"
#include "GUI/Menu.h"
#include "GUI/Arduino.h"
#include "Transmission.h"

int main(int argc, char *argv[]) {

	int  program_state = MENU;

	while(program_state != EXIT){
		
		switch(program_state){

			case GAME:{
				Control control;
				program_state = control.handle();
			} break;
			
			case CALIBRATION:{
				Calibration calibration;
				program_state = calibration.calibrate();
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
				program_state = MENU;
			} break;
			
			case EXIT:{
				Transmission transmission;
				program_state = transmission.closeTransmission();
			} break;
		}
	}

	return 0;
}