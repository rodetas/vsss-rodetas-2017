#ifndef ARDUINO_H_
#define ARDUINO_H_

#include "../Header.h"
#include "widgets/Button.h"

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include "../utils/Structs.h"

using namespace rod;

class Arduino{

private:

	tgui::Gui gui;
	tgui::Theme::Ptr theme;
	tgui::ComboBox::Ptr comboFile;
	tgui::ComboBox::Ptr comboPort;
	tgui::ComboBox::Ptr comboBaudRate;

	sf::Font font;

	sf::Text textFile;
	sf::Text textPort;
	sf::Text textBaudRate;

	Button btn_submit;
	Button btn_monitor;
	Button btn_update;

	vector<string> file_names;
	vector<string> port_names;
	vector<string> baudrate_names;

public:
	Arduino();
	int loop();
	void draw();
	void getFiles();
	void getPorts();
	void getBaudRates();
	void loadInformations();
	void deleteCompiled();
	void setCombos();
};

#endif