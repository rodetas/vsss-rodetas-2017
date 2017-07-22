#include "ArduinoModel.h"
#include "../view/ArduinoView.h"

ArduinoModel::ArduinoModel(){
}

ArduinoModel::~ArduinoModel(){
}

void ArduinoModel::init(ArduinoView* caller){
	getFiles(caller);
	getPorts(caller);
	getBaudRates(caller);
}

void ArduinoModel::upload(int n_file, int n_port, int n_rate){
	{
		std::lock_guard<std::mutex> lock(mutex);
		
		string active_file_name = file_names[n_file];
		string active_port = port_names[n_port];
		string active_baud_rate = baudrate_names[n_rate];

		string cmd = "cd ../files/arduino/" + active_file_name;
			cmd += " && sed -i 's;/dev/ttyUSB.*;" + active_port + ";' Makefile";
			cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + active_baud_rate + ";' Makefile";
			cmd += " && make monitor";

		system(cmd.c_str());
	}
}

void ArduinoModel::getFiles(ArduinoView* caller){
    {
		std::lock_guard<std::mutex> lock(mutex);

		file_names.clear();

		string cmd = "cd ../files/arduino && ls";
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
	caller->notify();
}

void ArduinoModel::getPorts(ArduinoView* caller){
	{
		std::lock_guard<std::mutex> lock(mutex);

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
	caller->notify();
}

void ArduinoModel::getBaudRates(ArduinoView* caller){
	{
		std::lock_guard<std::mutex> lock(mutex);
		baudrate_names.clear();
		baudrate_names.push_back("9600");
		baudrate_names.push_back("19200");
		baudrate_names.push_back("28800");
		baudrate_names.push_back("38400");
		baudrate_names.push_back("115200");
	}
	caller->notify();
}

void ArduinoModel::getData(vector<string> *file_names_p, vector<string> *port_names_p, vector<string> *baudrate_names_p) const {
	std::lock_guard<std::mutex> lock(mutex);

	if (file_names_p)
		*file_names_p = file_names;

	if (port_names_p)
		*port_names_p = port_names;

	if (baudrate_names_p)
		*baudrate_names_p = baudrate_names;
}

/*
void ArduinoModel::deleteCompiled(){
	string cmd = "find files/arduino/ -type d -name \"build-*\" -exec rm -r \"{}\" \\; ";
	system(cmd.c_str());
}
*/