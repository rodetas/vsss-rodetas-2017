#ifndef VISION_H_
#define VISION_H_

#include "Header.h"
#include "utils/OpenCV.h"
#include "utils/Timer.h"
#include "CRUD/Manipulation.h"

class Vision : public OpenCV {
private:

	Manipulation manipulation;
	Timer timer;

	const int number_robots = 3;

	vector<rodetas::Object> lastTeam;
	vector<rodetas::Object> robotTeam;
	vector<rodetas::Object> robotOpponent;
	rodetas::Object objectBall;
	
	cv::Mat opencvImageHSV;

    vector<Hsv> colorsHSV;
    vector<float> blobSize;

public:
	Vision();
	void initialize();
	
	void makeVision();
	rodetas::Object makeRobot(BlobsContours, BlobsContours, int);

	void colorPositionBall(BlobsContours);
	void colorPositionOpponent(BlobsContours);
	void colorPositionPlayer(cv::Mat, BlobsContours);

	BlobsContours orderBlobsContours(BlobsContours);

	vector<rodetas::Object> getPositions();
};
#endif