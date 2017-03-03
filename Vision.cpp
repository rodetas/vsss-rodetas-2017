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
    player_position = colorPositionPlayer(opencv_image_BGR, team_position);
    robotTeam = robotPosition();

    //Opponent
    opponent_position = position(opencv_image_BGR, opponent_position, colorsHSV[OPPONENT], 3);

    //Ball
    ball_position = position(opencv_image_BGR, ball_position, colorsHSV[BALL], 1);
}

/*
 * method to join team color with the player to create a robot
 */
vector<rodetas::Object> Vision::robotPosition(){

    vector<rodetas::Object> robot;

        for (int i = 0; i < player_position.center.size(); i++){
            int number_team = player_position.order[i];

            if (rodetas::insideCircle(player_position.center[i], team_position.center[number_team], team_position.radius[number_team] * 1.1 )){
                rodetas::Object r;
                r.x = team_position.center[number_team].x;
                r.y = team_position.center[number_team].y;
                r.angle = atan2 ((player_position.center[i].y - team_position.center[number_team].y),
                                ( player_position.center[i].x - team_position.center[number_team].x)) * (180 / CV_PI) + 180 + 45;
                robot.push_back(r);

            } else {
                cout << "COLOR IS NOT INSIDE CIRCLE - RADIUS: " <<  team_position.radius[number_team] * 1.1;
                cout << " Distance: " << rodetas::distance(player_position.center[i], team_position.center[number_team]) << endl;
            }
        }

    return robot;
}

/*
 * Method for find the color position of the player and make the robots
 */
ContoursPosition Vision::colorPositionPlayer(cv::Mat image, ContoursPosition team_position){

    ContoursPosition player_position;

    // cropped image around the color team
    for (int i = 0; i < team_position.center.size(); i++){

        // multplying 1.05 and 0.95, to cut closer the robot
        Point2f cutPoint1 = cv::Point( (team_position.cutPoint1[i].x)  * 1.05, (team_position.cutPoint1[i].y)  * 1.05);
        Point2f cutPoint2 = cv::Point( (team_position.cutPoint2[i].x)  * 0.95, (team_position.cutPoint2[i].y)  * 0.95);

        cv::Mat image_cut = opencvCutImage(image, cutPoint1, cutPoint2);
                image_cut = opencvRotateImage(image_cut, angle_image);
                image_cut = opencvColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                
        // search player's color on a cropped image
        for (int j = 0; j < number_robots; j++){
            cv::Mat image_binary = opencvBinary(image_cut, colorsHSV[j]);
            ContoursPosition find_position = findPosition(image_binary, 1);
            
            // check if finds the specified color in image
            if (find_position.center.size() != 0){
                // sum team cutPoint because the cut of the 
                Point2f position = cv::Point(find_position.center[0].x + cutPoint1.x, find_position.center[0].y + cutPoint1.y);
                player_position.center.push_back(position);
                player_position.radius.push_back(find_position.radius[0]);
                player_position.order.push_back(i);
                break;
            }
        }
    }
    
    return player_position;
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