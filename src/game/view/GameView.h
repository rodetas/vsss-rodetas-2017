#ifndef V_GAME_H_
#define V_GAME_H_

#include "../../Header.h"
#include "../model/GameModel.h"
#include "GameDraw.h"
#include <gtkmm.h>

class GameView {

public:
    GameView();
    virtual ~GameView();

    int GUI();
    void updateScreen(); 

private:
    GameModel game_model;

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
	bool deleteConnection(GdkEventAny*);

	sigc::connection control_connection;

	// criando sinal com parâmetros
	sigc::signal<void, vector<Robot>, vector<Robot>, Ball> signal_draw_robot;
};

#endif