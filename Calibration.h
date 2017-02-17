#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "Header.h"
#include "utils/Structs.h"
#include "utils/OpenCV.h"
#include "CRUD/Manipulation.h"
#include "GUI/Cairo/DrawAreaCalibration.h"

#include <gtkmm.h>
#include <thread>

using namespace rodetas;

class Calibration : public OpenCV {

private:

	Manipulation manipulation;
	CameraConfiguration camera_config;
	
	vector<Hsv> colorsHSV;
	vector<Rgb> colorsRGB;

    int camera;
	int program_state;
	int selected_player;
    bool end_calibration;
	bool camera_on;
	bool cairo_binary_image;
	double angle_image;
	
	//OPENCV
	cv::VideoCapture 	cam;
    cv::Mat 			opencv_image_BGR_cuted;
    cv::Mat 			opencv_image_BGR;
    cv::Mat 			opencv_image_HSV;
    cv::Mat 			opencv_image_cairo;
    cv::Mat 			opencv_image_binary;
	cv::Vec3b 			hsvPoint;
	cv::Vec3b 			rgbPoint;
	
	//GTKMM
	Glib::RefPtr<Gtk::Application> 	app;
	Gtk::ComboBoxText 				combo_choose_player;
	Gtk::RadioButton 				radio_button_image;
    Gtk::RadioButton 				radio_button_camera;
	Gtk::Button 					button_HSV_popover;
	Gtk::Button 					button_CAM_popover;
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
	bool updateScreen();
	void onMenuGame();
	void onMenuCalibration();
	void onMenuSimulator();
	void onMenuArduino();
	void onMenuRefresh();
	void onMenuQuit();	
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

	Point pointCutField1;
	Point pointCutField2;
    Point goal;

	Calibration();
	int calibrate();
	void defaultCameraConfig();
	void updateDevices();
	void updateColorPixel(Point);

	void imageInitialize();
	void imageWebCam();
	void getCalibration();

	void setCameraOn(bool);
	void setPopoverHSVDefault();
	void setValuesCamPopOver();

};
#endif