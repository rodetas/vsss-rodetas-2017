#ifndef V_CALIBRATION_H_
#define V_CALIBRATION_H_

#include "../../Header.h"
#include "../model/CalibrationModel.h"
#include "../DrawAreaCalibration.h"
#include <gtkmm.h>
#include <thread>


class CalibrationView {

public:
	CalibrationView();
	~CalibrationView();
    
    int GUI();
    void notify(string);

private:
	Glib::Dispatcher screen_connection;
	Glib::Dispatcher hsv_default_connection;

    CalibrationModel calibration_model;
    std::thread m_thread;
    int program_state;
	bool cairo_binary_image;

    Glib::RefPtr<Gtk::Application> 	app;
	Gtk::Window* window = nullptr;
	Gtk::MenuItem* menu_play = nullptr;
	Gtk::MenuItem* menu_simulator = nullptr;
	Gtk::MenuItem* menu_arduino = nullptr;
	Gtk::MenuItem* menu_quit = nullptr;
	Gtk::MenuItem* menu_save_calibration = nullptr;
	Gtk::MenuItem* menu_cut_image = nullptr;
	Gtk::MenuItem* menu_reset_values = nullptr;
	Gtk::MenuItem* menu_device0 = nullptr;
    Gtk::MenuItem* menu_device1 = nullptr;
	Gtk::MenuItem* menu_load_camera_config = nullptr;
	Gtk::MenuItem* menu_refresh = nullptr;
	Gtk::RadioButton* radio_button_image = nullptr;
    Gtk::RadioButton* radio_button_camera = nullptr;
	Gtk::ComboBoxText* combo_choose_player = nullptr;
	Gtk::Button* button_hsv_popover = nullptr;
	Gtk::Button* button_cam_popover = nullptr;
	Gtk::Popover* popover_hsv = nullptr;
	Gtk::Popover* popover_cam = nullptr;
	Gtk::Scale* scale_hmax = nullptr;
	Gtk::Scale* scale_hmin = nullptr;
	Gtk::Scale* scale_smax = nullptr;
	Gtk::Scale* scale_smin = nullptr;
	Gtk::Scale* scale_vmax = nullptr;
	Gtk::Scale* scale_vmin = nullptr;
	Gtk::Scale* scale_brightness = nullptr;
	Gtk::Scale* scale_contrast = nullptr;
	Gtk::Scale* scale_saturation = nullptr;
	Gtk::Scale* scale_gain = nullptr;
	Gtk::Scale* scale_sharpness = nullptr;
	Gtk::Scale* scale_exposure = nullptr;
    Gtk::Scale*	scale_rotate = nullptr;
	Gtk::Label* label_fps = nullptr;
	Gtk::Box* box_global = nullptr;
	DrawAreaCalibration draw_area;

    void updateScreen();
    void onMenuGame();
	void onMenuSimulator();
	void onMenuArduino();
	void onMenuQuit();
    void onSaveCalibration();
    void setPopoverHSVDefault();
    
    bool onMouseClick(GdkEventButton*);
};

#endif