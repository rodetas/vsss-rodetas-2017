#include "Arduino.h"

Arduino::Arduino(){
}

Arduino::~Arduino(){
}

int Arduino::GUI(){

	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("src/arduino/Arduino.glade");
	
	builder->get_widget("Window Arduino", window);
	
	builder->get_widget("Button Upload", btnUpload);
	btnUpload->signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonUpload) );
	
	builder->get_widget("Button Monitor", btnMonitor);
	btnMonitor->signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonMonitor) );
	
	builder->get_widget("Button Update", btnUpdate);
	btnUpdate->signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonUpdate) );
	
	builder->get_widget("ComboBoxText File", comboFile);
	builder->get_widget("ComboBoxText Port", comboPort);
	builder->get_widget("ComboBoxText BaudRate", comboBaudRate);

	loadInformations();
	setCombos();

	app->run(*window);

	return MENU;
}

void Arduino::onButtonUpload(){
	string cmd = "cd files/arduino/" + file_names[comboFile->get_active_row_number()];
		cmd += " && sed -i 's;/dev/ttyUSB.*;" + port_names[comboPort->get_active_row_number()] + ";' Makefile";
		cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + baudrate_names[comboBaudRate->get_active_row_number()] + ";' Makefile";
		cmd += " && make upload";

	system(cmd.c_str());
	deleteCompiled();	
}

void Arduino::onButtonMonitor(){
	string cmd = "cd files/arduino/" + file_names[comboFile->get_active_row_number()];
		cmd += " && sed -i 's;/dev/ttyUSB.*;" + port_names[comboPort->get_active_row_number()] + ";' Makefile";
		cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + baudrate_names[comboBaudRate->get_active_row_number()] + ";' Makefile";
		cmd += " && make monitor";

	system(cmd.c_str());
}

void Arduino::onButtonUpdate(){
	loadInformations();
	setCombos();
}

void Arduino::loadInformations(){
	getFiles();
	getPorts();
	getBaudRates();
}

void Arduino::getFiles(){
    
    file_names.clear();

    string cmd = "cd files/arduino && ls";
    string output = rodetas::executeCommand(cmd);

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
	string output = rodetas::executeCommand(cmd);

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
	baudrate_names.push_back("115200");
}


void Arduino::deleteCompiled(){
	string cmd = "find files/arduino/ -type d -name \"build-*\" -exec rm -r \"{}\" \\; ";
	system(cmd.c_str());
}


void Arduino::setCombos(){

	comboFile->remove_all();
	for(int i=0 ; i<file_names.size() ; i++){
	    comboFile->append(file_names[i]);
    }
    comboFile->set_active_text(file_names[0]);

    comboPort->remove_all();
    for(int i=0 ; i<port_names.size() ; i++){
	    comboPort->append(port_names[i]);
    }
    comboPort->set_active_text(port_names[0]);

    comboBaudRate->remove_all();
    for(int i=0 ; i<baudrate_names.size() ; i++){
	    comboBaudRate->append(baudrate_names[i]);
    }
    comboBaudRate->set_active_text(baudrate_names[0]); // active 9600
}