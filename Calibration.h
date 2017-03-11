#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "Header.h"
#include "utils/Structs.h"
#include "utils/OpenCV.h"
#include "utils/Timer.h"
#include "CRUD/Manipulation.h"
#include "GUI/Cairo/DrawAreaCalibration.h"

#include <gtkmm.h>
#include <thread>

using namespace rodetas;

class Calibration : public OpenCV {

private:

	Manipulation manipulation;
	Timer timer;
	
	vector<Hsv> colorsHSV;
	vector<Rgb> colorsRGB;

	int program_state;
	int selected_player;
    bool end_calibration;
	bool cairo_binary_image;
    Point goal;
	
	//OPENCV
    cv::Mat 			opencv_image_BGR_cuted;
    cv::Mat 			opencv_image_HSV;
    cv::Mat 			opencv_image_cairo;
    cv::Mat 			opencv_image_binary;
	cv::Vec3b 			hsvPoint;
	cv::Vec3b 			rgbPoint;

	//thread
	std::mutex mutex;
	cv::Mat thread_opencv_image_binary;
	cv::Mat thread_opencv_image_cairo;
	int thread_fps;
	
	//GTKMM
	Glib::RefPtr<Gtk::Application> 	app;
	Gtk::ComboBoxText 				combo_choose_player;
	Gtk::RadioButton 				radio_button_image;
    Gtk::RadioButton 				radio_button_camera;
	Gtk::Button 					button_HSV_popover;
	Gtk::Button 					button_CAM_popover;
	Gtk::Label						label_fps;
	Gtk::Popover 					HSV_popover;
	Gtk::Popover 					CAM_popover;
    Gtk::Scale 						scale_rotate;
    Gtk::ImageMenuItem 				menu_device0;
    Gtk::ImageMenuItem 				menu_device1;
	vector<Gtk::Scale> 				scale_CAM_popover;
    vector<Gtk::Scale> 				scale_HSV_popover;
    vector<Gtk::ImageMenuItem> 		vec_devices;
	DrawAreaCalibration 			draw_area;
	
	bool onMouseClick(GdkEventButton*);
	bool onKeyboard(GdkEventKey*);
	bool setInformations50MilliSec();
	void setThreadVariables();
	void onMenuGame();
	void onMenuSimulator();
	void onMenuArduino();
	void onMenuQuit();	
	void onMenuRefresh();
	void onButtonHSV();
	void onButtonCAM();
    void onScaleRotate();
    void onScaleHMax();
    void onScaleHMin();
    void onScaleSMax();
    void onScaleSMin();
    void onScaleVMax();
    void onScaleVMin();
    void onScaleCAMBrightness();
	void onScaleCAMContrast();
	void onScaleCAMSaturation();
	void onScaleCAMGain();
	void onScaleCAMSharpness();
	void onScaleCAMExposure();    
	void onChoosePlayer();
	void onRadioButtonImage();
	void onRadioButtonCamera();
	

public:

	void GUI();

	Calibration();
	int calibrate();
	void updateDevices();
	void updateColorPixel(Point);

	void getCalibration();

	void setCameraOn(bool);
	void setPopoverHSVDefault();
	void setPopoverCamValues();

};
#endif