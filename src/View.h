#ifndef VIEW_H_
#define VIEW_H_

#include "Header.h"
#include "Model.h"
#include <gtkmm.h>

class View {

public:
	View();
    ~View();
    
    int GUI();
    void notify(string);
    
protected:
    Model model;
    
    int program_state;
   
    sigc::connection loop_connection;

    Glib::RefPtr<Gtk::Application> 	app;
    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Window* window = nullptr;
    Gtk::MenuItem* menu_play = nullptr;
    Gtk::MenuItem* menu_calibration = nullptr;
	Gtk::MenuItem* menu_simulator = nullptr;
	Gtk::MenuItem* menu_arduino = nullptr;
    Gtk::MenuItem* menu_quit = nullptr;

    bool onKeyboard(GdkEventKey*);
    
    void onMenuGame(){
        program_state = GAME; window->close();
    };

    void onMenuCalibration(){
        program_state = CALIBRATION; window->close();
    };
    
    void onMenuSimulator(){
        program_state = SIMULATOR; window->close();
    };
    
    void onMenuArduino(){
        program_state = ARDUINO; window->close();
    };
    
    void onMenuQuit(){
        program_state = MENU; window->close();
    };

    void init(string glade){
        app = Gtk::Application::create();
        builder = Gtk::Builder::create_from_file(glade);

        builder->get_widget("Window Calibration", window);
        window->signal_key_press_event().connect(sigc::mem_fun(this, &View::onKeyboard));
        window->signal_key_release_event().connect(sigc::mem_fun(this, &View::onKeyboard), false);

        builder->get_widget("Menu Play", menu_play);
        menu_play->signal_activate().connect(sigc::mem_fun(this, &View::onMenuGame));

        builder->get_widget("Menu Calibrate", menu_calibration);
        menu_calibration->signal_activate().connect(sigc::mem_fun(this, &View::onMenuCalibration));    
        
        builder->get_widget("Menu Simulator", menu_simulator);
        menu_simulator->signal_activate().connect(sigc::mem_fun(this, &View::onMenuSimulator));
        
        builder->get_widget("Menu Arduino", menu_arduino);
        menu_arduino->signal_activate().connect(sigc::mem_fun(this, &View::onMenuArduino));
        
        builder->get_widget("Menu Quit", menu_quit);
        menu_quit->signal_activate().connect(sigc::mem_fun(this, &View::onMenuQuit));       
    };

};

#endif