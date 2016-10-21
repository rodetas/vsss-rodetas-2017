#include "Arduino.h"

Arduino::Arduino(){
	font.loadFromFile("files/fonts/fontText.ttf");
	loadInformations();
	draw();
}

int Arduino::loop(){

	sf::RenderWindow window(sf::VideoMode(400, 200), "Arduino Interface");
	gui.setWindow(window);

	while(window.isOpen()){

		window.clear(sf::Color(4,185,4));

		window.draw(textFile);
		window.draw(textPort);
		window.draw(textBaudRate);
		window.draw(btn_submit);
		window.draw(btn_monitor);
		window.draw(btn_update);

		sf::Event event;
	    while (window.pollEvent(event)){

	    	if (btn_submit.buttonEvent(event)){
	    		string cmd = "cd files/arduino/" + file_names[comboFile->getSelectedItemIndex()];
	    		cmd += " && sed -i 's;/dev/ttyUSB.*;" + port_names[comboPort->getSelectedItemIndex()] + ";' Makefile";
	    		cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + baudrate_names[comboBaudRate->getSelectedItemIndex()] + ";' Makefile";
	    		cmd += " && make upload";
	    		system(cmd.c_str());
	    		deleteCompiled();

	    	} else if(btn_monitor.buttonEvent(event)){
	    		string cmd = "cd files/arduino/" + file_names[comboFile->getSelectedItemIndex()];
	    		cmd += " && sed -i 's;/dev/ttyUSB.*;" + port_names[comboPort->getSelectedItemIndex()] + ";' Makefile";
	    		cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + baudrate_names[comboBaudRate->getSelectedItemIndex()] + ";' Makefile";
				cmd += " && make monitor";
	    		system(cmd.c_str());
	    		cout << cmd << endl;

	    	} else if(btn_update.buttonEvent(event)){
	    		loadInformations();
	    		setCombos();

	    	} else if(event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape){
	        	window.close();
	        } 

	    	gui.handleEvent(event);
	    }

        gui.draw();
        window.display();

    }
	return MENU;
}

void Arduino::draw(){
	theme = std::make_shared<tgui::Theme>(tguiThemePath);

	comboFile = theme->load("ComboBox");
	comboFile->setSize(250, 21); 

	comboPort = theme->load("ComboBox");
	comboPort->setSize(250, 21);

    comboBaudRate = theme->load("ComboBox");
    comboBaudRate->setSize(250, 21);

    setCombos();

    btn_submit.setPosition(95, 150);
    btn_monitor.setPosition(205, 150);
    btn_update.setPosition(315, 150);

    btn_submit.rectangleSize(80, 40);
    btn_monitor.rectangleSize(80, 40);
    btn_update.rectangleSize(80,40);

    btn_submit.textButton("Upload", 15);
    btn_monitor.textButton("Monitor", 15);
    btn_update.textButton("Refresh", 15);

    textFile.setFont(font);
    textPort.setFont(font);
    textBaudRate.setFont(font);

    textFile.setString("Sketch:");
    textPort.setString("Port:");
    textBaudRate.setString("Baud Rate:");

    textFile.setCharacterSize(15);
    textPort.setCharacterSize(15);
    textBaudRate.setCharacterSize(15);

    textFile.setPosition(7, 10);
    textPort.setPosition(7, 50);
    textBaudRate.setPosition(7,90);

    comboFile->setPosition(90, 10);
    comboPort->setPosition(90, 50);
    comboBaudRate->setPosition(90, 90);

    gui.add(comboFile, "comboFile");
    gui.add(comboPort, "comboPort");
    gui.add(comboBaudRate, "comboBaudRate");
}

void Arduino::getFiles(){
    
    file_names.clear();

    string cmd = "cd files/arduino && ls";
    string output = executeCommand(cmd);

    if(output.size() == 0){
    	file_names.push_back("NOT FOUND");

    } else {

	    string aux;
	    for(int i=0 ; i<output.size() ; i++){
	    	if(output[i] == '\n'){
	    		file_names.insert(file_names.begin(), aux);
	    		aux = "";
	    	} else {
	    		aux += output[i];
	    	}
	    }
	}
}

void Arduino::getPorts(){

	port_names.clear();

	string cmd = "find /dev/ -maxdepth 1 -name ttyUSB*";
	string output = executeCommand(cmd);

	if(output.size() == 0){
		port_names.push_back("NOT FOUND");

	} else {
		string aux;
		for(int i=0 ; i<output.size() ; i++){
			if(output[i] == '\n'){
				port_names.insert(port_names.begin(), aux);
				aux = "";
			} else {
				aux += output[i];
			}
		}
	}
}

void Arduino::getBaudRates(){
	baudrate_names.clear();
	baudrate_names.push_back("9600");
	baudrate_names.push_back("19200");
	baudrate_names.push_back("28800");
	baudrate_names.push_back("38400");
}

void Arduino::loadInformations(){
	getFiles();
	getPorts();
	getBaudRates();
}

void Arduino::deleteCompiled(){
	string cmd = "find files/arduino/ -type d -name \"build-*\" -exec rm -r \"{}\" \\; ";
	system(cmd.c_str());
}

void Arduino::setCombos(){

	comboFile->removeAllItems();
	for(int i=0 ; i<file_names.size() ; i++){
	    comboFile->addItem(file_names[i]);
    }
    comboFile->setSelectedItem(file_names[0]);

    comboPort->removeAllItems();
    for(int i=0 ; i<port_names.size() ; i++){
	    comboPort->addItem(port_names[i]);
    }
    comboPort->setSelectedItem(port_names[0]);

    comboBaudRate->removeAllItems();
    for(int i=0 ; i<baudrate_names.size() ; i++){
	    comboBaudRate->addItem(baudrate_names[i]);
    }
    comboBaudRate->setSelectedItem(baudrate_names[1]);
}