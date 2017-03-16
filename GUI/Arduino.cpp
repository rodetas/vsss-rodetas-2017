#include "Arduino.h"

Arduino::Arduino(){
	loadInformations();
	setCombos();
}

int Arduino::GUI(){

	auto app = Gtk::Application::create();

	draw();

	show_all_children();
	app->run(*this);

	close();
	return MENU;
}

void Arduino::onButtonUpload(){
	string cmd = "cd files/arduino/" + file_names[comboFile.get_active_row_number()];
		cmd += " && sed -i 's;/dev/ttyUSB.*;" + port_names[comboPort.get_active_row_number()] + ";' Makefile";
		cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + baudrate_names[comboBaudRate.get_active_row_number()] + ";' Makefile";
		cmd += " && make upload";

	system(cmd.c_str());
	deleteCompiled();	
}

void Arduino::onButtonMonitor(){
	string cmd = "cd files/arduino/" + file_names[comboFile.get_active_row_number()];
		cmd += " && sed -i 's;/dev/ttyUSB.*;" + port_names[comboPort.get_active_row_number()] + ";' Makefile";
		cmd += " && sed -i 's;MONITOR_BAUDRATE.*;MONITOR_BAUDRATE = " + baudrate_names[comboBaudRate.get_active_row_number()] + ";' Makefile";
		cmd += " && make monitor";

	system(cmd.c_str());
}

void Arduino::onButtonUpdate(){
	loadInformations();
	setCombos();
}

void Arduino::draw(){
	set_default_size(400, 250);
	set_icon_from_file("files/images/logo-rodetas.png");
	set_position(Gtk::WIN_POS_CENTER_ALWAYS);
	set_title("Arduino Interface");

	comboFile.set_size_request(200,-1);

	textFile.set_size_request(80, -1);
	textPort.set_size_request(80, -1);
	textBaudRate.set_size_request(80, -1);

	textFile.set_alignment(Gtk::ALIGN_START);
	textPort.set_alignment(Gtk::ALIGN_START);
	textBaudRate.set_alignment(Gtk::ALIGN_START);

	textFile.set_text("Sketch:");
	textPort.set_text("Port:");
	textBaudRate.set_text("Baud Rate:");

	btnUpload.signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonUpload) );
	btnMonitor.signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonMonitor) );
	btnUpdate.signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonUpdate) );

	btnUpload.add_label("Upload");
	btnMonitor.add_label("Monitor");
	btnUpdate.add_label("Refresh");

	buttonBox.set_layout(Gtk::BUTTONBOX_CENTER);
	buttonBox.set_spacing(20);
	buttonBox.pack_start(btnUpload);
	buttonBox.pack_start(btnMonitor);
	buttonBox.pack_start(btnUpdate);

	grid.set_row_spacing(15);

	grid.attach(textFile,0,0,1,1);
	grid.attach(textPort,0,1,1,1);
	grid.attach(textBaudRate,0,2,1,1);

	grid.attach(comboFile,1,0,1,1);
	grid.attach(comboPort,1,1,1,1);
	grid.attach(comboBaudRate,1,2,1,1);

	grid.attach(buttonBox,0,3,3,1);

	grid.set_valign(Gtk::ALIGN_CENTER);
	grid.set_halign(Gtk::ALIGN_CENTER);

	add(grid);
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

	comboFile.remove_all();
	for(int i=0 ; i<file_names.size() ; i++){
	    comboFile.append(file_names[i]);
    }
    comboFile.set_active_text(file_names[0]);

    comboPort.remove_all();
    for(int i=0 ; i<port_names.size() ; i++){
	    comboPort.append(port_names[i]);
    }
    comboPort.set_active_text(port_names[0]);

    comboBaudRate.remove_all();
    for(int i=0 ; i<baudrate_names.size() ; i++){
	    comboBaudRate.append(baudrate_names[i]);
    }
    comboBaudRate.set_active_text(baudrate_names[1]);
}
