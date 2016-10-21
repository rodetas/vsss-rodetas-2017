#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "Header.h"
#include "utils/Structs.h"
#include "utils/OpenCV.h"
#include "GUI/GUICalibration.h"
#include "CRUD/Manipulation.h"

#include <fstream>

using namespace rod;

class Calibration : public OpenCV {

private:

	GUICalibration gui;
	Manipulation manipulation;
	
	cv::VideoCapture cam;
	cv::Mat opencvImageBGR;
	cv::Mat opencvImageHSV;
	cv::Mat opencvImageBGRCuted;
	cv::Mat opencvImageBinary;
	cv::Mat opencvBGRtoRGB;

	cv::Vec3b hsvPoint;
	cv::Vec3b rgbPoint;

	vector<Hsv> colorsHSV;
	vector<Rgb> colorsRGB;
	vector<float> blobSize;

	Point resolutionCamera;
    bool calibrationWasOpen;

    int camera;

public:

	Point imageSize;
	Point pointCutField1;
	Point pointCutField2;
	bool endCalibration;
	bool cameraOn;
	bool changedColor;
    int selectedTab;
	int angleImageRotation;
    vector<int> range;
    Point imageSizeZoom;
    Point goal;
    Point margin;

	Calibration();
	void calibrate();
	void openWindow();

	void updateRGB();
	void updateHSV();
	void updateColorPixel(Point);
	void updateTab();
	void updateSlider();

	sf::VertexArray drawBlobs(cv::Mat);
	Point changeCordinates(Point);
	void imageInitialize();
	void imageWebCam();
	void imageCanCut();
	void emptyImage();
	void closeConectionCamera();
	void pointCutFieldDefault();

	void getCalibration();
	void setGUICalibration(Calibration*);

};
#endif