#ifndef VISION_H_
#define VISION_H_

#include "Header.h"
#include "utils/OpenCV.h"
#include "utils/Timer.h"
#include "CRUD/Manipulation.h"
#include <thread>


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

public:
	Vision();
	void initialize();	
	void computerVision();
	void colorPositionPlayer(cv::Mat, ContoursPosition);

	void teamThread();
	void opponentThread();
	void ballThread();

	rodetas::Object robotPosition(ContoursPosition, int);
	vector<rodetas::Object> getPositions();
};
#endif