#ifndef M_CALIBRATION_H_
#define M_CALIBRATION_H_

#include "../../Header.h"
#include "../../utils/Timer.h"
#include "../../utils/OpenCV.h"
#include "../../utils/Commons.h"
#include "../../utils/Manipulation.h"

class CalibrationView;
class CalibrationModel : public OpenCV {

public:
	CalibrationModel();
	~CalibrationModel();
	
	void saveParameters();
	void updateColorPixel(Point, Point);
	bool updateFrame();
	void setCaller(CalibrationView*);

	int getFps();
	cv::Mat getScreenImage();
	cv::Mat getScreenBinaryImage();

private:

	Timer timer;
	Manipulation manipulation;
	CameraConfiguration camera_config;
	CalibrationView* caller;
	
	Point goal;
	PointCut point_cut;
	
	vector<Rgb> colorsRGB;
	vector<Hsv> colorsHSV;
	
	int fps;
	int angle_image;
	int selected_player;
	bool camera_on;
	bool cairo_binary_image;
	
	cv::Mat opencv_image_BGR;
	cv::Mat opencv_image_HSV;
    cv::Mat opencv_image_cairo;
    cv::Mat opencv_image_binary;
	cv::Mat opencv_image_BGR_cuted;
	cv::Mat opencv_image_BGR_rotated;
	cv::Vec3b hsv_point;
	cv::Vec3b rgb_point;
};

#endif