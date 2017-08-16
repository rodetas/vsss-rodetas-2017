#ifndef V_GAME_H_
#define V_GAME_H_

#include "../../Header.h"
#include "../model/GameModel.h"
#include "DrawAreaControl.h"
#include <gtkmm.h>

class GameView {

public:
    GameView();
    ~GameView();

    int GUI();
    void updateScreen();
    void notify(string);    

private:
    GameModel game_model;

    sigc::connection control_connection;

    Glib::RefPtr<Gtk::Application> app;    
    Gtk::Window *window = nullptr;
	Gtk::ToggleButton *button_play = nullptr;
	Gtk::ToggleButton *button_penalty = nullptr;
	Gtk::ToggleButton *button_side = nullptr;
	Gtk::SpinButton *spin_potency = nullptr;
	Gtk::SpinButton *spin_curve  = nullptr;
	Gtk::Label *label_fps = nullptr;
	Gtk::Label *label_transmission = nullptr;
	Gtk::MenuItem *menu_calibration = nullptr;
	Gtk::MenuItem *menu_simulator = nullptr;
	Gtk::MenuItem *menu_arduino = nullptr;
	Gtk::MenuItem *menu_quit = nullptr;
	Gtk::SeparatorMenuItem *separator = nullptr;
    Gtk::Box *box = nullptr;
    DrawAreaControl draw_robot;
    
    int program_state;

    void onMenuCalibration();
	void onMenuSimulator();
	void onMenuArduino();
    void onMenuQuit();
    void onButtonPlay();
	void onPotencyChanged();
	void onCurveChanged();
	void onButtonTime();    
    bool onKeyboard(GdkEventKey*);    
};

#endif