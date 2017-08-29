#ifndef VISION_H_
#define VISION_H_

#include "../Header.h"
#include "../utils/OpenCV.h"
#include "../utils/Camera.h"
#include "../utils/Manipulation.h"

class Vision : public OpenCV {
private:

	Manipulation manipulation;
	CameraConfiguration camera_config;	

	const int number_robots = 3;

    vector<Hsv> colorsHSV;
	vector<rodetas::Object> robotTeam;

	int angle_image;
	bool camera_on;
	PointCut point_cut;
	cv::Mat opencv_image_BGR;

	Position color_team_position;
    Position color_ball_position;
    Position color_opponent_position;

public:
	Vision();
	void initialize();	
	void computerVision();

	void teamPosition(Position, cv::Mat);
    void ballPosition(Position);
    void opponentPosition(Position);

	vector<rodetas::Object> getPositions();
};
#endif