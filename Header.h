#ifndef HEADER_H_
#define HEADER_H_

#define RADIAN_TO_DEGREE (180/CV_PI)


#define MENU 0
#define GAME 1
#define CALIBRATION 2
#define SIMULATOR 3
#define ARDUINO 4
#define TEST 5
#define EXIT 6

#define SAVE_COORD
#define CAMERACONFIG
#define FPS 30
//#define camera 0

#define robot1InitialCharacter '('
#define robot1FinalCharacter ')'
#define robot2InitialCharacter '{'
#define robot2FinalCharacter '}'
#define robot0InitialCharacter '['
#define robot0FinalCharacter ']'
#define FORWARD_MOVE "A"
#define BACK_MOVE "V"
#define RIGHT_MOVE "D"
#define LEFT_MOVE "E"
#define STOPPED_MOVE "P"

#define imagePath "files/images/imageTest1(640x480).png"
#define backgroundPath "files/textures/background.jpg"
#define backgroundPath_menu "files/textures/background_menu.jpg"
#define textureBallPath "files/textures/ball.png"
#define fontTitlePath "files/fonts/fontTitle.ttf"
#define fontTextPath "files/fonts/fontText.ttf"
#define tguiThemePath "files/tguiThemes/Black.txt"
#define instancePath "files/instances/fileCalibration.txt"
#define textureButtonPressed "files/textures/button2.png"
#define textureButtonNonPressed "files/textures/button1.png"
#define textureButtonHover "files/textures/button3.png"

#include <iostream>
#include <vector>

using namespace std;

#endif
