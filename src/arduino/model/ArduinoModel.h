#ifndef M_ARDUINO_H_
#define M_ARDUINO_H_

#include "../../Header.h"
#include "../../utils/Commons.h"
#include <gtkmm.h>

class ArduinoView;
class ArduinoModel {

public:
	ArduinoModel();
	~ArduinoModel();
	
	void init();
	void upload(int, int, int);
	void getFiles();
	void getPorts();
	void getBaudRates();
	void getData(vector<string>*, vector<string>*, vector<string>*) const;	
	void setCaller(ArduinoView* c);

private:
	ArduinoView* caller;

	vector<string> file_names;
	vector<string> port_names;
	vector<string> baudrate_names;
};

#include "../view/ArduinoView.h"

#endif