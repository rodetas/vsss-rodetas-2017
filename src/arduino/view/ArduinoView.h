#ifndef V_ARDUINO_H_
#define V_ARDUINO_H_

#include "../../Header.h"
#include "../../utils/Commons.h"
#include "../model/ArduinoModel.h"
#include <gtkmm.h>
#include <thread>

class ArduinoView {

public:
	ArduinoView();
	~ArduinoView();

	int GUI();

	//void setModel(ArduinoModel*);

private:

	//ArduinoModel* model;

	Glib::RefPtr<Gtk::Application> app;
	Gtk::Window* window = nullptr;
	Gtk::Button* btnUpload = nullptr;
	Gtk::Button* btnMonitor = nullptr;
	Gtk::Button* btnUpdate = nullptr;
	Gtk::ComboBoxText* comboFile = nullptr;
	Gtk::ComboBoxText* comboPort = nullptr;
	Gtk::ComboBoxText* comboBaudRate = nullptr;

	std::thread* model_thread = nullptr;

	vector<string> file_names;
	vector<string> port_names;
	vector<string> baudrate_names;


	void getData(string&, string&, string&);

	//void onButtonUpload();
	//void onButtonMonitor();
	//void onButtonUpdate();
};

#endif