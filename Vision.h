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
	
    vector<Hsv> colorsHSV;
	
	ContoursPosition ball_position;
	ContoursPosition team_position;
	ContoursPosition opponent_position;

public:
	Vision();
	void initialize();
	
	void computerVision();
	rodetas::Object robotPosition(ContoursPosition, ContoursPosition, int);
	void colorPositionPlayer(cv::Mat, ContoursPosition);

	ContoursPosition orderContoursPosition(ContoursPosition);

	vector<rodetas::Object> getPositions();
};
#endif