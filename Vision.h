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

	int camera;
	int angleImageRotation;
	int numberOfRobots;
	int rotateField;

	vector<Object> lastTeam;
	vector<Object> robotTeam;
	vector<Object> robotOpponent;
	Object objectBall;
	Object lastBall;

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
	Object makeRobot(BlobsContours, BlobsContours, int);

	void colorPositionBall(BlobsContours);
	void colorPositionOpponent(BlobsContours);
	void colorPositionPlayer(cv::Mat, BlobsContours);

	BlobsContours orderBlobsContours(BlobsContours);

	void initializeWebcam();
	void imageWebCam();
	void setRotateField(bool);

	Object getBall();
	vector<Object> getRobotTeam();
	vector<Object> getOpponent();

};
#endif