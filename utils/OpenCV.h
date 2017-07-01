#ifndef OPENCV_H_
#define OPENCV_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "Commons.h"
#include "Timer.h"

using namespace rodetas;

class OpenCV {

private:
	Timer timer;
	int frames = 0;
	float percent_cut;

	cv::VideoCapture cam;	
		
public:
	OpenCV();
	cv::Mat binarize(cv::Mat, Hsv);
	cv::Mat changeColorSpace(cv::Mat, int);
	cv::Mat cutImage(cv::Mat, PointCut);
	cv::Mat rotateImage(cv::Mat, int);
	cv::Mat updateCameraImage();
	cv::Mat imageInitialize();
	cv::Mat cameraInitialize(CameraConfiguration, int);
	void imageValidation(cv::Mat, PointCut);
	void cameraRelease();

	ContoursPosition position(cv::Mat, ContoursPosition, Hsv, int);
	ContoursPosition binarizedColorPosition(cv::Mat, int n_contours);
};
#endif