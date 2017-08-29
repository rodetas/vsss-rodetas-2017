#ifndef OPENCV_H_
#define OPENCV_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "Commons.h"
#include "Timer.h"
#include "Camera.h"

using namespace rodetas;

class OpenCV {

private:
	cv::VideoCapture cam;
	Timer timer;

protected:
	Camera camera;
	CameraConfiguration camera_config;
		
public:
	OpenCV();
	cv::Mat binarize(cv::Mat, Hsv);
	cv::Mat changeColorSpace(cv::Mat, int);
	cv::Mat cutImage(cv::Mat, PointCut);
	cv::Mat rotateImage(cv::Mat, int);
	cv::Mat imageInitialize();
	cv::Mat cameraInitialize();
	cv::Mat cameraUpdate();
	void cameraRelease();
	void frameValidation(cv::Mat, PointCut);
	cv::Mat frameInitialize(bool);

	Position position(cv::Mat, Hsv, int);
	Position position(cv::Mat, Hsv, int, PointCut);
	Position binarizedColorPosition(cv::Mat, int n_contours);
};
#endif