#ifndef VISION_H_
#define VISION_H_

#include "../Header.h"
#include "../utils/OpenCV.h"
#include "../utils/Camera.h"
#include "../utils/Timer.h"
#include "../utils/Manipulation.h"

#include <thread>
#include <mutex>


class Vision : public OpenCV {
private:

	Manipulation manipulation;

	const int number_robots = 3;

    vector<Hsv> colorsHSV;
	vector<rodetas::Object> robotTeam;
	
	ContoursPosition ball_position;
	ContoursPosition team_position;
	ContoursPosition opponent_position;

	cv::Mat full_image_cut;

	CameraConfiguration camera_config;	
	cv::Mat opencv_image_BGR;

	std::mutex mutex;

	int angle_image;
	int camera_number;
	bool camera_on;
	bool image_initialize;
	bool camera_initialize;
	Point goal;
	PointCut point_cut;


public:
	Vision();
	void initialize();	
	void computerVision();
	void colorPositionPlayer(cv::Mat, ContoursPosition);

	void teamThread();
	void opponentThread();
	void ballThread();

	rodetas::Object robotPosition(ContoursPosition, int);

	vector<rodetas::Object>& getTeam();
	vector<rodetas::Object> getOpponent();
	rodetas::Object getBall();

	vector<rodetas::Object> getPositions();
};
#endif