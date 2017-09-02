#include "Vision.h"

/*
 * Constructor of the class
 */
Vision::Vision(){
    n_robots = 3;

    robot_team.resize(n_robots);
    robot_opponent.resize(n_robots);

    for (int i = 0; i < n_robots; i++){
        robot_team[i].setRobotId(i);
        robot_opponent[i].setRobotId(i);
    }
}

/*
 * Method that initialize the instance
 */
void Vision::initialize(){
    manipulation.loadCalibration();  
    colorsHSV           = manipulation.getColorsHsv();
    camera_on           = manipulation.getCameraOn();
    point_cut.first     = manipulation.getPointField1();
    point_cut.second    = manipulation.getPointField2();
    angle_image         = manipulation.getAngleImage();
    camera_config       = manipulation.loadCameraConfig();
    
    camera.setCameraValuesScript(camera_config);    

    if (camera_on) {
        opencv_image_BGR = cameraInitialize();
    } else {
        opencv_image_BGR = imageInitialize();
    }
}

/*
 * Calls the functions to make robots
 */
void Vision::computerVision(){
    
    if (camera_on)
        opencv_image_BGR = cameraUpdate();

    cv::Mat full_image_cut = rotateImage(opencv_image_BGR, angle_image);
            full_image_cut = cutImage(full_image_cut, point_cut);

    cv::imwrite("./teste.jpg", full_image_cut);
    
    Position color_team_position = position(full_image_cut, colorsHSV[TEAM], 3);
    Position color_ball_position = position(full_image_cut, colorsHSV[BALL], 1);
    Position color_opponent_position = position(full_image_cut, colorsHSV[OPPONENT], 3);

    teamPosition(color_team_position, full_image_cut);
    ballPosition(color_ball_position);
    opponentPosition(color_opponent_position);
}

void Vision::teamPosition(Position team_position, cv::Mat image){

    // OTIMIZAR
    for(int i=0 ; i<n_robots ; i++){
        robot_team[i].setPosition(Point(0,0));
    }

    for (int i = 0; i < team_position.size(); i++){
        // set the cutpoint around the color team
        PointCut cutPoint(team_position.center[i], team_position.radius[i]);
        
        Robot robot(i);
        float biggest_radius = 0;
        
        for (int j = 0; j < n_robots; j++){
            // search player's color on a cutpoint area           
            Position find_position = position(image, colorsHSV[j], 1, cutPoint);

            if (find_position.size() > 0 && find_position.radius[0] > biggest_radius){
                
                robot.setRobotId(j);
                robot.setPosition(team_position.center[i]);
                robot.setAngle( atan2 ( (find_position.center[0].y - team_position.center[i].y), (find_position.center[0].x - team_position.center[i].x)) * (180 / CV_PI) + 180 + 45 );
            
                biggest_radius = find_position.radius[0];
            }
        }

        robot_team[robot.getRobotId()].setPosition(robot.getPosition());
        robot_team[robot.getRobotId()].setAngle(robot.getAngle());
    }
}

void Vision::opponentPosition(Position opponent_position){
    for(int i=0 ; i<n_robots ; i++){
        robot_opponent[i].setPosition(Point(0,0));
    }
    for (int i = 0; i < opponent_position.size(); i++){
        robot_opponent[i].setPosition(opponent_position.center[i]);
    }
/*  REFAZER ESTA PARTE COM A CLASSE ROBOT
    Position opponent_position_aux;
        Position last_opponent_position = opponent_position;
        opponent_position = position(full_image_cut, colorsHSV[OPPONENT], 3);
        
        int position;

        for(int i=0; i<last_opponent_position.size();i++){
            long double distance = rodetas::distance(last_opponent_position.center[i],opponent_position.center[0]);
            
            for(int j=0; j<opponent_position.size();j++){
                long double distance_aux = rodetas::distance(last_opponent_position.center[i],opponent_position.center[j]);
                if(distance_aux <= distance){
                    distance = distance_aux;
                    position = j;
                }
            }
            opponent_position_aux.center.push_back(opponent_position.center[position]);
            opponent_position_aux.radius.push_back(opponent_position.radius[position]);
        
        }
        
        if(last_opponent_position.center.size()!=0){
            opponent_position = opponent_position_aux;
        }    
*/
}

void Vision::ballPosition(Position ball_position){
    if(ball_position.size() > 0)
        ball.setPosition(ball_position.center[0]);
    else
        ball.setPosition(Point(0,0));
}

vector<Robot> Vision::getTeam(){
    return robot_team;
}

vector<Robot> Vision::getOpponent(){
    return robot_opponent;
}

Ball Vision::getBall(){
    return ball;
}