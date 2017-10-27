#ifndef VISION_H_
#define VISION_H_

#include "../Header.h"
#include "../utils/Ball.h"
#include "../utils/Robot.h"
#include "../utils/Camera.h"
#include "../utils/OpenCV.h"
#include "../utils/Manipulation.h"

class Vision : public OpenCV {
private:

	Manipulation manipulation;
	CameraConfiguration camera_config;	
	
	Ball* ball;
	PointCut point_cut;
	cv::Mat opencv_image_BGR;

	vector<Hsv> colorsHSV;
	vector<Robot*> robot_team;
	vector<Robot*> robot_opponent;
	
	int n_robots;
	int angle_image;
	bool camera_on;
	bool game_side;
	
	void teamPosition(Position, cv::Mat);
    void ballPosition(Position);
    void opponentPosition(Position);

public:
	Vision();
	void initialize(vector<Robot*>, vector<Robot*>, Ball*);
	void computerVision();
	void setGameSide();
	
};
#endif