#ifndef M_ARDUINO_H_
#define M_ARDUINO_H_

#include "../../Header.h"
#include "../../utils/Commons.h"
#include "../view/ArduinoView.h"
#include <gtkmm.h>

class ArduinoModel {

public:
	ArduinoModel();
	~ArduinoModel();

	//void notify(ArduinoView*);
	void notify();
	void upload(string, string, string);

	//void getFiles();
	//void getPorts();
	//void getBaudRates();
	//void deleteCompiled();
	//void setCombos();

private:
	string active_file_name;
	string active_port;
	string active_baud_rate;
};

#endif