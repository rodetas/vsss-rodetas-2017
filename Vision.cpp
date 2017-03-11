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

    full_image_cut = opencvRotateImage(opencv_image_BGR, angle_image);
    full_image_cut = opencvCutImage(full_image_cut, point_cut_field_1, point_cut_field_2);

    std::thread team_thread(&Vision::teamThread, this);
    std::thread ball_thread(&Vision::ballThread, this);
    std::thread opponent_thread(&Vision::opponentThread, this);

    team_thread.join();
    ball_thread.join();
    opponent_thread.join();
}

void Vision::teamThread(){
    team_position = position(full_image_cut, team_position, colorsHSV[TEAM], 3);
    colorPositionPlayer(full_image_cut, team_position);
}

void Vision::opponentThread(){
    opponent_position = position(full_image_cut, opponent_position, colorsHSV[OPPONENT], 3);
}

void Vision::ballThread(){
    ball_position = position(full_image_cut, ball_position, colorsHSV[BALL], 1);
}

/*
 * method to join team color with the player to create a robot
 */
 rodetas::Object Vision::robotPosition(ContoursPosition color_player_position, int number_team){

    rodetas::Object robot;

        if (rodetas::insideCircle(color_player_position.center[0], team_position.center[number_team], team_position.radius[number_team] * 1.1 )){
            robot.x = team_position.center[number_team].x;
            robot.y = team_position.center[number_team].y;
            robot.angle = atan2 ((color_player_position.center[0].y - team_position.center[number_team].y),
                                 (color_player_position.center[0].x - team_position.center[number_team].x)) * (180 / CV_PI) + 180 + 45;
        } else {
            cout << "COLOR IS NOT INSIDE CIRCLE - RADIUS: " <<  team_position.radius[number_team] * 1.1;
            cout << " Distance: " << rodetas::distance(color_player_position.center[0], team_position.center[number_team]) << endl;
        }

    return robot;
}

/*
 * Method for find the color position of the player and make the robots
 */
void Vision::colorPositionPlayer(cv::Mat image, ContoursPosition team_position){

    vector<rodetas::Object> robot(number_robots);

    // cropped image around the color team
    for (int i = 0; i < team_position.center.size(); i++){

        Point2f cutPoint1 = cv::Point( team_position.center[i].x - team_position.radius[i] , team_position.center[i].y - team_position.radius[i] );
        Point2f cutPoint2 = cv::Point( team_position.center[i].x + team_position.radius[i] , team_position.center[i].y + team_position.radius[i] );

        cv::Mat image_cut = opencvCutImage(image, cutPoint1, cutPoint2);
                image_cut = opencvColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                
        float biggest_radius = 0;

        // search player's color on a cropped image
        for (int j = 0; j < number_robots; j++){
            cv::Mat image_binary = opencvBinary(image_cut, colorsHSV[j]);
            ContoursPosition find_position = findPosition(image_binary, 1);
            
            // check if finds the specified color in image
            if (find_position.center.size() != 0){
                if (find_position.radius[0] > biggest_radius){

                    find_position.center[0].x += cutPoint1.x;
                    find_position.center[0].y += cutPoint1.y;
                    robot[j] = robotPosition(find_position, i);
                    
                    biggest_radius = find_position.radius[0];
                }
            }
        }
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