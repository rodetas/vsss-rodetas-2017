#include "ArduinoView.h"

ArduinoView::ArduinoView(){
	arduino_model.init(this);
}

ArduinoView::~ArduinoView(){

}

int ArduinoView::GUI(){

	app = Gtk::Application::create();

	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("arduino/view/Arduino.glade");
	
	builder->get_widget("Window Arduino", window);
	window->signal_key_press_event().connect(sigc::mem_fun(this, &ArduinoView::onKeyboard));
	
	
	builder->get_widget("Button Upload", buttonUpload);
	buttonUpload->signal_clicked().connect( sigc::mem_fun(*this, &ArduinoView::onButtonUpload) );
	
	//builder->get_widget("Button Monitor", buttonMonitor);
	//buttonMonitor->signal_clicked().connect( sigc::mem_fun(this, &ArduinoView::onButtonMonitor) );
	
	builder->get_widget("Button Update", buttonUpdate);
	buttonUpdate->signal_clicked().connect( sigc::mem_fun(this, &ArduinoView::onButtonUpdate) );
	
	builder->get_widget("ComboBoxText File", comboFile);
	builder->get_widget("ComboBoxText Port", comboPort);
	builder->get_widget("ComboBoxText BaudRate", comboBaudRate);

	m_dispatcher.connect(sigc::mem_fun(*this, &ArduinoView::onNotify));

	app->run(*window);

	return MENU;
}

bool ArduinoView::onKeyboard(GdkEventKey* event){

    if(event->keyval == GDK_KEY_Escape){
        //program_state = MENU; window->close();
    }

    return true;    
}

void ArduinoView::onButtonUpdate(){
	arduino_model.init(this);
}

void ArduinoView::onButtonUpload(){
	// Nesse caso a thread não é necessária, a linha abaixo é o suficiente
	//arduino_model.upload(comboFile->get_active_row_number(), comboPort->get_active_row_number(), comboBaudRate->get_active_row_number()); 
	
	buttonUpload->set_sensitive(false);
	m_thread = std::thread([this] {
		arduino_model.upload(comboFile->get_active_row_number(), comboPort->get_active_row_number(), comboBaudRate->get_active_row_number()); 
		buttonUpload->set_sensitive(true);			
	});
	m_thread.detach();
}

void ArduinoView::updateWidgets(){
	vector<string> file_names;
	vector<string> port_names;
	vector<string> baudrate_names;

	arduino_model.getData(&file_names, &port_names, &baudrate_names);
	
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

void ArduinoView::notify(){
	m_dispatcher.emit();
}

void ArduinoView::onNotify(){
	updateWidgets();
}