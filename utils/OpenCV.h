#ifndef OPENCV_H_
#define OPENCV_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "Structs.h"
#include "Timer.h"

using namespace rodetas;

class OpenCV {

private:
	Timer timer;

protected:
	CameraConfiguration camera_config;
	
	cv::VideoCapture cam;
	cv::Mat opencv_image_BGR;
	
	bool camera_on;
	int angle_image;
	int camera_number;
	Point point_cut_field_1;
	Point point_cut_field_2;

public:
	OpenCV();
	cv::Mat opencvBinary(Hsv, cv::Mat);
	cv::Mat opencvColorSpace(cv::Mat, int);
	cv::Mat opencvTransformation(cv::Mat, int, cv::Point, cv::Point);
	void imageInitialize(bool);
	void imageWebCam(bool);
	void setCameraRelease();
	
	BlobsContours blobContour(cv::Mat, float);
	float blobRadius(cv::Mat image);
};
#endif