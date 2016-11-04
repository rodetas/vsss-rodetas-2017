#ifndef VISION_H_
#define VISION_H_

#include "Header.h"
#include "utils/OpenCV.h"
#include "CRUD/Manipulation.h"

class Vision : public OpenCV {
private:

	Manipulation manipulation;

	bool cameraOn;
	bool begin;

	const int number_robots = 3;

	int camera;
	int angleImageRotation;
	int rotateField;

	vector<rod::Object> lastTeam;
	vector<rod::Object> robotTeam;
	vector<rod::Object> robotOpponent;
	rod::Object objectBall;

	Point pointCutField1;
	Point pointCutField2;
	
	cv::VideoCapture cam;
	cv::Mat opencvImageHSV;
	cv::Mat opencvImageBGR;

    vector<Hsv> colorsHSV;
    vector<float> blobSize;

public:
	Vision();
	void initialize();
	void setCameraRelease();
	
	void makeVision();
	rod::Object makeRobot(BlobsContours, BlobsContours, int);

	void colorPositionBall(BlobsContours);
	void colorPositionOpponent(BlobsContours);
	void colorPositionPlayer(cv::Mat, BlobsContours);

	BlobsContours orderBlobsContours(BlobsContours);

	void initializeWebcam();
	void imageWebCam();

	vector<rod::Object> getPositions();
};
#endif