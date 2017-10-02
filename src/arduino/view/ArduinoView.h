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
	void notify();
	
private:
	ArduinoModel arduino_model;
	Glib::Dispatcher m_dispatcher;
	
	Gtk::Window* window = nullptr;
	Gtk::Button* buttonUpload = nullptr;
	Gtk::Button* buttonUpdate = nullptr;
	//Gtk::Button* buttonMonitor = nullptr;
	Gtk::ComboBoxText* comboFile = nullptr;
	Gtk::ComboBoxText* comboPort = nullptr;
	Gtk::ComboBoxText* comboBaudRate = nullptr;
	
	void updateWidgets();
	void onButtonUpload();
	void onButtonUpdate();
	//void onButtonMonitor();
	
	bool onKeyboard(GdkEventKey*);
};

#endif