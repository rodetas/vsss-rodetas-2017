#ifndef M_ARDUINO_H_
#define M_ARDUINO_H_

#include "../../Header.h"
#include "../../utils/Commons.h"
#include <gtkmm.h>
#include <mutex>

class ArduinoView;
class ArduinoModel {

public:
	ArduinoModel();
	~ArduinoModel();
	
	void init(ArduinoView*);
	void upload(int, int, int);
	void getFiles(ArduinoView*);
	void getPorts(ArduinoView*);
	void getBaudRates(ArduinoView*);
	void getData(vector<string>*, vector<string>*, vector<string>*) const;	
	//void deleteCompiled();

private:
	mutable std::mutex mutex;

	vector<string> file_names;
	vector<string> port_names;
	vector<string> baudrate_names;
};

#include "../view/ArduinoView.h"

#endif