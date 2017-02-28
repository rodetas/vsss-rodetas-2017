#ifndef OPENCV_H_
#define OPENCV_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "Structs.h"
#include "Timer.h"

using namespace rodetas;

class OpenCV {

private:
	Timer timer;
	bool review_all_image;

protected:
	CameraConfiguration camera_config;
	
	cv::VideoCapture cam;
	cv::Mat opencv_image_BGR;
	
	int angle_image;
	int camera_number;
	bool camera_on;
	bool image_initialize;
	bool camera_initialize;
	Point point_cut_field_1;
	Point point_cut_field_2;
	
public:
	OpenCV();
	cv::Mat opencvBinary(cv::Mat, Hsv);
	cv::Mat opencvColorSpace(cv::Mat, int);
	cv::Mat opencvCutImage(cv::Mat, cv::Point, cv::Point);
	cv::Mat opencvRotateImage(cv::Mat, int);
	void setImage();
	void imageInitialize();
	void cameraRelease();
	void cameraInitialize();
	void imageValidation();

	ContoursPosition findPosition(cv::Mat, int n_contours);

	ContoursPosition position(cv::Mat, ContoursPosition, Hsv, int);
};
#endif