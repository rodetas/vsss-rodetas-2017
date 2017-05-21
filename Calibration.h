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
#include <mutex>

using namespace rodetas;

class Calibration : public OpenCV {

private:

	Manipulation manipulation;
	Timer timer;
	
	vector<Hsv> colorsHSV;
	vector<Rgb> colorsRGB;

	int program_state;
	int selected_player;
	bool cairo_binary_image;
	
	//OPENCV    
	cv::Vec3b 			hsvPoint;
	cv::Vec3b 			rgbPoint;

	//thread
	std::mutex mutex;
	
	//GTKMM
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
	
	bool onMouseClick(GdkEventButton*);
	bool onKeyboard(GdkEventKey*);
	bool setInformations50MilliSec();
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
	
	void setEndCalibration(bool);
	bool getEndCalibration();
    bool end_calibration;

	void setPointCutField1(Point);
	Point getPointCutField1();

	void setPointCutField2(Point);
	Point getPointCutField2();

	void setGoal(Point);
	Point getGoal();

	void setAngleImage(int);
	int getAngleImage();

	void setCameraOn(bool);
	bool getCameraOn();

	void setOpenCVImageBGR(cv::Mat);
	cv::Mat getOpencvImageBGR();

	void setOpenCVImageBGRCuted(cv::Mat);
	cv::Mat getOpencvImageBGRCuted();
	cv::Mat opencv_image_BGR_cuted;

	void setOpenCVImageHSV(cv::Mat);
	cv::Mat getOpencvImageHSV();
    cv::Mat opencv_image_HSV;

	void setOpenCVImageCairo(cv::Mat);
	cv::Mat getOpencvImageCairo();
    cv::Mat opencv_image_cairo;

	void setOpenCVImageBinary(cv::Mat);
	cv::Mat getOpencvImageBinary();
    cv::Mat opencv_image_binary;

public:

	int GUI();

	Calibration();
	void thread();
	void updateDevices();
	void updateColorPixel(Point);

	void getCalibration();

	void setPopoverHSVDefault();
	void setPopoverCamValues();

};
#endif