#include "Vision.h"

/*
 * Constructor of the class
 */
Vision::Vision(){
    colorsHSV.resize(6);
    blobSize.resize(6);
    robotTeam.resize(number_robots);
    robotOpponent.resize(number_robots);
    
    camera = getCameraNumber();
}

/*
 * Method that initialize the instance
 */
void Vision::initialize(){
    manipulation.loadCalibration();  
    colorsHSV = manipulation.getColorsHsv();
    camera_on = manipulation.getCameraOn();
    pointCutField1 = manipulation.getPointField1();
    pointCutField2 = manipulation.getPointField2();
    angleImageRotation = manipulation.getAngleImage();
    blobSize = manipulation.getBlobSize();

    //manipulation.showCalibration();

    imageInitialize();
}

/*
 * Calls the functions to make robots
 */
void Vision::makeVision(){

    imageWebCam();

    opencvImageHSV = opencvColorSpace(opencvTransformation(opencv_image_BGR, angleImageRotation + rotateField, pointCutField1, pointCutField2), cv::COLOR_BGR2HSV_FULL);

    //Team
    BlobsContours teamContours = blobContour( opencvBinary(colorsHSV[team], opencvImageHSV), blobSize[team]);
    colorPositionPlayer(opencvImageHSV, teamContours);

    //Opponent
    BlobsContours opponentContours = blobContour( opencvBinary(colorsHSV[opponent], opencvImageHSV), blobSize[opponent]);
    colorPositionOpponent(opponentContours);

    //Ball
    colorPositionBall( blobContour( opencvBinary(colorsHSV[ball], opencvImageHSV), blobSize[ball]));
}

/*
 * method to join team color with the player to create a robot
 */
rodetas::Object Vision::makeRobot(BlobsContours playerContours, BlobsContours teamContours, int numberTeam){

    rodetas::Object robot;

    for (int i = 0; i < playerContours.center.size(); i++){
        playerContours.center[i].x += teamContours.cutPoint1[numberTeam].x;
        playerContours.center[i].y += teamContours.cutPoint1[numberTeam].y;

        float distance = rodetas::distance(playerContours.center[i], teamContours.center[numberTeam]);

        if (distance <= teamContours.radius[numberTeam] && distance >= teamContours.radius[numberTeam]/6){
            robot.x = teamContours.center[numberTeam].x;
            robot.y = teamContours.center[numberTeam].y;
            robot.angle = atan2 ((playerContours.center[i].y - teamContours.center[numberTeam].y),
                                 (playerContours.center[i].x - teamContours.center[numberTeam].x)) * (180 / CV_PI) + 180 + 45;
        }
    }

    return robot;
}

/*
 * Method for find the color position of the player and make the robots
 */
void Vision::colorPositionPlayer(cv::Mat image, BlobsContours teamContours){

    // can be less than 3 robots
    robotTeam.clear();
    robotTeam.resize(number_robots);

    // cropped image around the color team
    for (int i = 0; i < teamContours.center.size(); i++){
        cv::Mat cutImage = image( cv::Rect(teamContours.cutPoint1[i], teamContours.cutPoint2[i]) );

        // search player's color on a cropped image
        for (int j = 0; j < number_robots; j++){
            BlobsContours contours = blobContour( opencvBinary(colorsHSV[j], cutImage), blobSize[j]);

            // check if finds the specified color in image
            if (contours.center.size() != 0){
                robotTeam[j] = makeRobot(contours, teamContours, i);
                break;
            }
        }
    }
}

/*
 * Method for set the position of opponent's color
 */
void Vision::colorPositionOpponent(BlobsContours opponentContours){

    robotOpponent.clear();
    robotOpponent.resize(number_robots);

    for (int i = 0; i < opponentContours.center.size(); i++){
        robotOpponent[i].x = opponentContours.center[i].x;
        robotOpponent[i].y = opponentContours.center[i].y;
    }
}

/*
 * Method for set the position of ball's color
 */
void Vision::colorPositionBall(BlobsContours ballContours){

    objectBall.clear();

    int radiusBall = 0; 
    for(int i = 0; i < ballContours.center.size(); i++){ 
        if (ballContours.radius[i] >= radiusBall && ballContours.center.size() > 0){
            radiusBall = ballContours.radius[i];
            objectBall.x = int(ballContours.center[i].x);
            objectBall.y = int(ballContours.center[i].y);
        }
    }
}

/*
 * Method for initialize image
 */
void Vision::imageInitialize(){

    if(camera_on){
        cam = cv::VideoCapture(camera);
        timer.wait(10000); //time to camera answer
        if(cam.isOpened()){
            //cam.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
            //cam.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
            cam >> opencv_image_BGR;
        } else {
            cout << "Conection with camera failed" << endl;
        }

    } else {
        if(cam.isOpened()){ 
            cam.release();
        }

        opencv_image_BGR = cv::imread(imagePath);

        if(opencv_image_BGR.empty()){
            cout << "Problem to load image from computer" << endl;
        }
    }
}

/*
 * Method that receives and analyzes image from webcam
 */
void Vision::imageWebCam(){
     if(camera_on && cam.isOpened()){
        cam >> opencv_image_BGR;
    }
}

/*
 * Setters
 */
void Vision::setCameraRelease(){
    cam.release();
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
/*
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i].x == 0 || objects[i].y == 0) {
            objects[i].print();
        } 
    }
*/
    return objects;
} 