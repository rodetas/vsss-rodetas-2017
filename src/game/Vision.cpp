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
    point_cut.first     = manipulation.getPointField1();
    point_cut.second    = manipulation.getPointField2();
    angle_image         = manipulation.getAngleImage();
    camera_config       = manipulation.loadCameraConfig();
    camera_initialize   = manipulation.getCameraOn();
    image_initialize    = !manipulation.getCameraOn();

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

    full_image_cut = rotateImage(opencv_image_BGR, angle_image);
    full_image_cut = cutImage(full_image_cut, point_cut);

    std::thread team_thread(&Vision::teamThread, this);
    std::thread ball_thread(&Vision::ballThread, this);
    std::thread opponent_thread(&Vision::opponentThread, this);

    team_thread.join();
    ball_thread.join();
    opponent_thread.join();
}
//    int percent_to_cut = image.cols * 0.05;

void Vision::teamThread(){
    team_position = position(full_image_cut, colorsHSV[TEAM], 3);

    vector<rodetas::Object> robot(3);

    // cropped image around the color team
    for (int i = 0; i < team_position.size(); i++){

        PointCut cutPoint;
        cutPoint.first = cv::Point( team_position.center[i].x - team_position.radius[i] , team_position.center[i].y - team_position.radius[i] );
        cutPoint.second = cv::Point( team_position.center[i].x + team_position.radius[i] , team_position.center[i].y + team_position.radius[i] );

        cv::Mat image_cut = cutImage(full_image_cut, cutPoint);
                image_cut = changeColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                
        float biggest_radius = 0;

        // search player's color on a cropped image
        for (int j = 0; j < number_robots; j++){
            Position find_position = position(image_cut, colorsHSV[j], 1);
            
            // check if finds the specified color in image
            if (find_position.size() != 0){
                if (find_position.radius[0] > biggest_radius){
                    find_position.changeCoordinateToGlobal(0, cutPoint);
                    biggest_radius = find_position.radius[0];
                    
                    rodetas::Object r;
                        r.x = team_position.center[i].x;
                        r.y = team_position.center[i].y;
                        r.angle = atan2 ((find_position.center[0].y - team_position.center[i].y), (find_position.center[0].x - team_position.center[i].x)) * (180 / CV_PI) + 180 + 45;
                    
                    robot[j] = r;
                }
            }
        }
    }
    
    robotTeam = robot;
}

void Vision::opponentThread(){
    /*
    Position opponent_position_aux;
    Position last_opponent_position = opponent_position;
    opponent_position = position(full_image_cut, opponent_position, colorsHSV[OPPONENT], 3);
    
    int position;

    for(int i=0; i<last_opponent_position.center.size();i++){
         long double distance = rodetas::distance(last_opponent_position.center[i],opponent_position.center[0]);
         
         for(int j=0; j<opponent_position.center.size();j++){
             long double distance_aux = rodetas::distance(last_opponent_position.center[i],opponent_position.center[j]);
             if(distance_aux <= distance){
                 distance = distance_aux;
                 position = j;
             }
        }
        opponent_position_aux.center.push_back(opponent_position.center[position]);
        opponent_position_aux.cutPoint.push_back(opponent_position.cutPoint[position]);
        opponent_position_aux.radius.push_back(opponent_position.radius[position]);
    
    }
    
    if(last_opponent_position.center.size()!=0){
        opponent_position = opponent_position_aux;
    }
    */
    
}

void Vision::ballThread(){
    //ball_position = position(full_image_cut, ball_position, colorsHSV[BALL], 1);
}

vector<rodetas::Object>& Vision::getTeam(){
    return robotTeam;
}

vector<rodetas::Object> Vision::getOpponent(){
    vector<rodetas::Object> objects;
    objects.resize(0);

    // ALEXANDRE, FAZER UMA REVISAO AQUI
    // COMPARA COM A FORMA QUE ERA FEITA NO getPositions()
    for (int i = 0; i < opponent_position.center.size(); i++){
        objects.push_back(Object(opponent_position.center[i]));
    }

    return objects;
}

rodetas::Object Vision::getBall(){
    Object o;
    
    if(ball_position.center.size() != 0){
        o = Object(ball_position.center[0]);
    }

    return o;

}

vector<rodetas::Object> Vision::getPositions(){
    std::lock_guard<std::mutex> lock(mutex);
    
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