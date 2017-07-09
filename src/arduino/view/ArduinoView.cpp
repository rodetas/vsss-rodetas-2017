#include "ArduinoView.h"

ArduinoView::ArduinoView(){

}

ArduinoView::~ArduinoView(){

}

int ArduinoView::GUI(){

	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("arduino/Arduino.glade");
	
	builder->get_widget("Window Arduino", window);
	
	builder->get_widget("Button Upload", btnUpload);
	//btnUpload->signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonUpload) );
	
	builder->get_widget("Button Monitor", btnMonitor);
	//btnMonitor->signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonMonitor) );
	
	builder->get_widget("Button Update", btnUpdate);
	//btnUpdate->signal_clicked().connect( sigc::mem_fun(this, &Arduino::onButtonUpdate) );
	
	builder->get_widget("ComboBoxText File", comboFile);
	builder->get_widget("ComboBoxText Port", comboPort);
	builder->get_widget("ComboBoxText BaudRate", comboBaudRate);

	//loadInformations();
	//setCombos();

	app->run(*window);

	return MENU;
}

/*
void ArduinoView::onButtonUpload(){
	if (model_thread){
    	std::cout << "Can't start a worker thread while another one is running." << std::endl;
  	} else {
		// Start a new worker thread.
		model_thread = new std::thread([&]{
			model->upload(file_names[comboFile->get_active_row_number()], 
							port_names[comboPort->get_active_row_number()],
							baudrate_names[comboBaudRate->get_active_row_number()]);
		});
  	}
}
*/

/*
void ArduinoModel::onButtonUpdate(){
	loadInformations();
	setCombos();
}
*/

/*
void ArduinoModel::loadInformations(){
	getFiles();
	getPorts();
	getBaudRates();
}
*/

/*
void ArduinoView::setModel(ArduinoModel* m){
	model = m;
}
*/
void ArduinoView::getData(string& filename, string& portname, string& baudrate){
	filename = file_names[comboFile->get_active_row_number()];
	portname = port_names[comboPort->get_active_row_number()];
	baudrate = baudrate_names[comboBaudRate->get_active_row_number()];
}