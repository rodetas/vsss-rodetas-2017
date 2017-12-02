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
	Gtk::ToggleButton *button_side = nullptr;
	Gtk::ModelButton *button_information = nullptr;	
	Gtk::RadioButton* radio_button_robot_0 = nullptr;
    Gtk::RadioButton* radio_button_robot_1 = nullptr;
	Gtk::RadioButton* radio_button_robot_2 = nullptr;
	Gtk::Switch* switch_robot = nullptr;
	
	Gtk::Popover *popover_information = nullptr;	

	Gtk::SpinButton *spin_potency_robot0 = nullptr;
	Gtk::SpinButton *spin_potency_robot1 = nullptr;
	Gtk::SpinButton *spin_potency_robot2 = nullptr;
	Gtk::SpinButton *spin_curve_robot0  = nullptr;
	Gtk::SpinButton *spin_curve_robot1  = nullptr;
	Gtk::SpinButton *spin_curve_robot2  = nullptr;
	
	Gtk::Label *label_fps = nullptr;
	Gtk::Label *label_robot_0 = nullptr;
	Gtk::Label *label_robot_1 = nullptr;
	Gtk::Label *label_robot_2 = nullptr;
	Gtk::Label *label_ball = nullptr;
	Gtk::Label *label_transmission = nullptr;
	
	Gtk::MenuItem *menu_calibration = nullptr;
	Gtk::MenuItem *menu_simulator = nullptr;
	Gtk::MenuItem *menu_arduino = nullptr;
	Gtk::MenuItem *menu_quit = nullptr;
	Gtk::Box *box = nullptr;

	sigc::signal <Point> signal_draw_get_click;
	sigc::signal <void, GdkEventButton*> signal_press_event;
	    
    int program_state;

    void onMenuCalibration();
	void onMenuSimulator();
	void onMenuArduino();
    void onMenuQuit();
    void onButtonPlay();
	void onPotencyChangedRobot0();
	void onPotencyChangedRobot1();
	void onPotencyChangedRobot2();
	void onCurveChangedRobot0();
	void onCurveChangedRobot1();
	void onCurveChangedRobot2();
	void onSwitchRobot();
	void onButtonTime();
	void onRadioButton0();
	void onRadioButton1();
	void onRadioButton2();
	void onButtonInformation();   
	bool onMouseClick(GdkEventButton*);
	bool onKeyboard(GdkEventKey*);
	bool deleteConnection(GdkEventAny*);

	sigc::connection control_connection;

	// criando sinal com parâmetros
	sigc::signal<void, vector<Robot>, vector<Robot>, Ball> signal_draw_robot;
};

#endif