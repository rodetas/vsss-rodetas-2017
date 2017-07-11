#include "ArduinoModel.h"

ArduinoModel::ArduinoModel(){

}

ArduinoModel::~ArduinoModel(){

}
/*
void ArduinoModel::notify(ArduinoView* caller){
	caller->getData(&active_file_name, &active_port, &active_baud_rate);
}
*/
void ArduinoModel::upload(string file_name, string active_port, string baud_rate){
	string cmd = "cd files/arduino/" + file_name;
		cmd += " && sed -i 's;/dev/ttyUSB.*;" + active_port + ";' Makefile";
		cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + baud_rate + ";' Makefile";
		cmd += " && make monitor";

	system(cmd.c_str());
}


/*
void ArduinoModel::getFiles(){
    
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
*/

/*
void ArduinoModel::getPorts(){

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
*/

/*
void ArduinoModel::getBaudRates(){
	baudrate_names.clear();
	baudrate_names.push_back("9600");
	baudrate_names.push_back("19200");
	baudrate_names.push_back("28800");
	baudrate_names.push_back("38400");
	baudrate_names.push_back("115200");
}
*/

/*
void ArduinoModel::deleteCompiled(){
	string cmd = "find files/arduino/ -type d -name \"build-*\" -exec rm -r \"{}\" \\; ";
	system(cmd.c_str());
}
*/


/*
void ArduinoModel::setCombos(){
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
*/