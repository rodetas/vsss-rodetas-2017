#include "Vision.h"

/*
 * Constructor of the class
 */
Vision::Vision(){
    colorsHSV.resize(6);
    robotTeam.resize(number_robots);
    robotOpponent.resize(number_robots);
    
    camera_config = manipulation.loadCameraConfig();
}

/*
 * Method that initialize the instance
 */
void Vision::initialize(){
    manipulation.loadCalibration();  
    colorsHSV = manipulation.getColorsHsv();
    camera_on = manipulation.getCameraOn();
    point_cut_field_1 = manipulation.getPointField1();
    point_cut_field_2 = manipulation.getPointField2();
    angle_image = manipulation.getAngleImage();

    //manipulation.showCalibration();

    imageInitialize(camera_on);
}

/*
 * Calls the functions to make robots
 */
void Vision::computerVision(){

    imageWebCam(camera_on);

    opencv_image_HSV = opencvColorSpace(opencvTransformation(opencv_image_BGR, angle_image, point_cut_field_1, point_cut_field_2), cv::COLOR_BGR2HSV_FULL);

    //Team
    team_position = position(opencvBinary(opencv_image_HSV, colorsHSV[TEAM]));
    colorPositionPlayer(opencv_image_HSV, team_position);

    //Opponent
    opponent_position = position(opencvBinary(opencv_image_HSV, colorsHSV[OPPONENT]));
    colorPositionOpponent(opponent_position);

    //Ball
    colorPositionBall( position( opencvBinary(opencv_image_HSV, colorsHSV[BALL])));

    //cv::imshow("teste", opencvTransformation(opencv_image_BGR, angle_image, point_cut_field_1, point_cut_field_2));
    //cv::waitKey(10);
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
        }
    }   

    return robot;
}

/*
 * Method for find the color position of the player and make the robots
 */
void Vision::colorPositionPlayer(cv::Mat image, ContoursPosition team_position){

    // can be less than 3 robots
    robotTeam.clear();
    robotTeam.resize(number_robots);

    // cropped image around the color team
    for (int i = 0; i < team_position.center.size(); i++){
        cv::Mat cut_image = image( cv::Rect(team_position.cutPoint1[i], team_position.cutPoint2[i]) );

        // search player's color on a cropped image
        for (int j = 0; j < number_robots; j++){
            ContoursPosition color_player_position = position( opencvBinary(cut_image, colorsHSV[j]));

            // check if finds the specified color in image
            if (color_player_position.center.size() != 0){
                robotTeam[j] = robotPosition(color_player_position, team_position, i);
                break;
            }
        }
    }
}

/*
 * Method for set the position of opponent's color
 */
void Vision::colorPositionOpponent(ContoursPosition opponent_position){

    robotOpponent.clear();
    robotOpponent.resize(number_robots);

    for (int i = 0; i < opponent_position.center.size(); i++){
        robotOpponent[i].x = opponent_position.center[i].x;
        robotOpponent[i].y = opponent_position.center[i].y;
    }
}

/*
 * Method for set the position of ball's color
 */
void Vision::colorPositionBall(ContoursPosition ball_position){

    objectBall.clear();

    int radiusBall = 0; 
    for(int i = 0; i < ball_position.center.size(); i++){ 
        if (ball_position.radius[i] >= radiusBall && ball_position.center.size() > 0){
            radiusBall = ball_position.radius[i];
            objectBall.x = int(ball_position.center[i].x);
            objectBall.y = int(ball_position.center[i].y);
        }
    }
}

/*
 * Getters
 */
vector<rodetas::Object> Vision::getPositions(){
    
    vector<rodetas::Object> objects;

    for(int i = 0 ; i < robotTeam.size() ; i++)
        objects.push_back(robotTeam[i]);
    
    for(int i = 0 ; i < robotOpponent.size() ; i++)
        objects.push_back(robotOpponent[i]);
    
    objects.push_back(objectBall);

    for (int j = 0; j < number_robots; j++){
        if (robotTeam[j].x == 0 || robotTeam[j].y == 0){
            robotTeam[j].print();
        }
    }

    return objects;
}