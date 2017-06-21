#ifndef HEADER_H_
#define HEADER_H_

#define RADIAN_TO_DEGREE (180/CV_PI)

#define MENU 0
#define GAME 1
#define CALIBRATION 2
#define SIMULATOR 3
#define ARDUINO 4
#define EXIT 5

#define robot1InitialCharacter '('
#define robot1FinalCharacter ')'
#define robot2InitialCharacter '{'
#define robot2FinalCharacter '}'
#define robot0InitialCharacter '['
#define robot0FinalCharacter ']'

#define FORWARD_MOVE 'F'
#define BACK_MOVE 'B'
#define RIGHT_MOVE 'R'
#define LEFT_MOVE 'L'
#define STOPPED_MOVE 'S'

#define imagePath "files/images/1280 x 720.png"
// #define instancePath "files/instances/fileCalibration.txt"

#include <iostream>
#include <vector>

using namespace std;

#endif