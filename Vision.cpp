#include "Vision.h"

/*
 * Constructor of the class
 */
Vision::Vision(){
    robotTeam.resize(number_robots);
}

/*
 * Method that initialize the instance
 */
void Vision::initialize(){
    manipulation.loadCalibration();  
    colorsHSV           = manipulation.getColorsHsv();
    camera_on           = manipulation.getCameraOn();
    point_cut_field_1   = manipulation.getPointField1();
    point_cut_field_2   = manipulation.getPointField2();
    angle_image         = manipulation.getAngleImage();
    camera_config       = manipulation.loadCameraConfig();
    camera_initialize   = manipulation.getCameraOn();
    image_initialize    = !manipulation.getCameraOn();

    setImage();
}

/*
 * Calls the functions to make robots
 */
void Vision::computerVision(){

    setImage();

    //Team
    team_position = position(opencv_image_BGR, team_position, colorsHSV[TEAM], 3);
    colorPositionPlayer(opencv_image_BGR, team_position);

    //Opponent
    opponent_position = position(opencv_image_BGR, opponent_position, colorsHSV[OPPONENT], 3);

    //Ball
    ball_position = position(opencv_image_BGR, ball_position, colorsHSV[BALL], 1);
}

/*
 * method to join team color with the player to create a robot
 */
rodetas::Object Vision::robotPosition(ContoursPosition color_player_position, ContoursPosition team_position, int number_team){

    rodetas::Object robot;

    for (int i = 0; i < color_player_position.center.size(); i++){

        // sum team cutPoint because the cut of the image
        color_player_position.center[i].x += team_position.cutPoint1[number_team].x;
        color_player_position.center[i].y += team_position.cutPoint1[number_team].y;

        if (rodetas::insideCircle(color_player_position.center[i], team_position.center[number_team], team_position.radius[number_team] )){
            robot.x = team_position.center[number_team].x;
            robot.y = team_position.center[number_team].y;
            robot.angle = atan2 ((color_player_position.center[i].y - team_position.center[number_team].y),
                                 (color_player_position.center[i].x - team_position.center[number_team].x)) * (180 / CV_PI) + 180 + 45;
        } else {
            cout << "COLOR IS NOT INSIDE CIRCLE" << endl;
        }
    }   

    return robot;
}

/*
 * Method for find the color position of the player and make the robots
 */
void Vision::colorPositionPlayer(cv::Mat image, ContoursPosition team_position){
/*
    vector<rodetas::Object> robot(number_robots);

    // cropped image around the color team
    for (int i = 0; i < team_position.center.size(); i++){
        cv::Mat image_cut = opencvCutImage(image, team_position.cutPoint1[i], team_position.cutPoint2[i]);
                image_cut = opencvRotateImage(image_cut, angle_image);
                image_cut = opencvColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                
        // search player's color on a cropped image
        for (int j = 0; j < number_robots; j++){
            cv::Mat image_binary = opencvBinary(image_cut, colorsHSV[j]);
            ContoursPosition find_position = findPosition(image_binary, 1);

            // check if finds the specified color in image
            if (find_position.center.size() != 0){
                robot[j] = robotPosition(find_position, team_position, i);
                break;
            }
        }
    }
    
    robotTeam = robot;
*/
    vector<rodetas::Object> robot(number_robots);

    for (int i = 0; i < team_position.center.size(); i++){
        robot[i].x = team_position.center[i].x;
        robot[i].y = team_position.center[i].y;
    }

    robotTeam = robot;
}

/*
 * Getters
 */
vector<rodetas::Object> Vision::getPositions(){
    
    vector<rodetas::Object> objects(7);

    for(int i = 0 ; i < robotTeam.size() ; i++){
        objects[i] = robotTeam[i];
    }

    for (int i = number_robots; i < opponent_position.center.size() + number_robots; i++){
        objects[i].x = opponent_position.center[i-number_robots].x;
        objects[i].y = opponent_position.center[i-number_robots].y;
    }
    
    if (ball_position.center.size() > 0){
        objects[6].x = ball_position.center[0].x;
        objects[6].y = ball_position.center[0].y;
    }

    return objects;
}