#ifndef ARDUINO_H_
#define ARDUINO_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include <gtkmm.h>

class Arduino {

public:
	Arduino();
	~Arduino();

	int GUI();
	void getFiles();
	void getPorts();
	void getBaudRates();
	void loadInformations();
	void deleteCompiled();
	void setCombos();

private:

	Glib::RefPtr<Gtk::Application> app;
	Gtk::Window* window = nullptr;
	Gtk::Button* btnUpload = nullptr;
	Gtk::Button* btnMonitor = nullptr;
	Gtk::Button* btnUpdate = nullptr;
	Gtk::ComboBoxText* comboFile = nullptr;
	Gtk::ComboBoxText* comboPort = nullptr;
	Gtk::ComboBoxText* comboBaudRate = nullptr;

	vector<string> file_names;
	vector<string> port_names;
	vector<string> baudrate_names;

	void onButtonUpload();
	void onButtonMonitor();
	void onButtonUpdate();
};

#endif