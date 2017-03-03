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

	vector<rodetas::Object> robotTeam;
	
    vector<Hsv> colorsHSV;
	
	ContoursPosition ball_position;
	ContoursPosition team_position;
	ContoursPosition opponent_position;

public:
	Vision();
	void initialize();	
	void computerVision();
	void colorPositionPlayer(cv::Mat, ContoursPosition);

	rodetas::Object robotPosition(ContoursPosition, int);
	vector<rodetas::Object> getPositions();
};
#endif