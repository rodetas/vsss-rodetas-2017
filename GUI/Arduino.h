#ifndef ARDUINO_H_
#define ARDUINO_H_

#include "../Header.h"
#include "../utils/Structs.h"

#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/buttonbox.h>

class Arduino : public Gtk::Window{

public:
	Arduino();

	int GUI();
	void draw();
	void getFiles();
	void getPorts();
	void getBaudRates();
	void loadInformations();
	void deleteCompiled();
	void setCombos();

private:
	vector<string> file_names;
	vector<string> port_names;
	vector<string> baudrate_names;

	void onButtonUpload();
	void onButtonMonitor();
	void onButtonUpdate();

protected:
	Gtk::Grid grid;

	Gtk::ComboBoxText comboFile;
	Gtk::ComboBoxText comboPort;
	Gtk::ComboBoxText comboBaudRate;

	Gtk::Label textFile;
	Gtk::Label textPort;
	Gtk::Label textBaudRate;

	Gtk::ButtonBox buttonBox;

	Gtk::Button btnUpload;
	Gtk::Button btnMonitor;
	Gtk::Button btnUpdate;

};

#endif