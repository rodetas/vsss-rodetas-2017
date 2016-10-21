#ifndef OPENCV_H_
#define OPENCV_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "Structs.h"

using namespace rod;

class OpenCV {

private:

public:
	OpenCV();
	cv::Mat opencvBinary(Hsv, cv::Mat);
	cv::Mat opencvColorSpace(cv::Mat, int);
	cv::Mat opencvTransformation(cv::Mat, int, cv::Point, cv::Point, cv::Point);
	BlobsContours blobContour(cv::Mat, float);
	float blobRadius(cv::Mat image);
};
#endif