#include "Vision.h"

/*
 * Constructor of the class
 */
Vision::Vision(){
    colorsHSV.resize(6);
    blobSize.resize(6);
    robotTeam.resize(3);
    lastTeam.resize(3);
    robotOpponent.resize(3);
    numberOfRobots = 3;

    camera = getCameraNumber();
}

/*
 * Method that initialize the instance
 */
void Vision::initialize(){
    manipulation.loadCalibration();  
    colorsHSV = manipulation.getColorsHsv();
    cameraOn = manipulation.getCameraOn();
    pointCutField1 = manipulation.getPointField1();
    pointCutField2 = manipulation.getPointField2();
    angleImageRotation = manipulation.getAngleImage();
    blobSize = manipulation.getBlobSize();

    //manipulation.showCalibration();

    initializeWebcam();
}

/*
 * Calls the functions to make robots
 */
void Vision::makeVision(){

    imageWebCam();

    opencvImageHSV = opencvColorSpace(opencvTransformation(opencvImageBGR, angleImageRotation + rotateField, pointCutField1, pointCutField2, Point(0,0)), cv::COLOR_BGR2HSV_FULL);

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
Object Vision::makeRobot(BlobsContours playerContours, BlobsContours teamContours, int numberTeam){

    Object robot;

    for (int i = 0; i < playerContours.center.size(); i++){
        playerContours.center[i].x += teamContours.cutPoint1[numberTeam].x;
        playerContours.center[i].y += teamContours.cutPoint1[numberTeam].y;

        float distance = rod::distance(playerContours.center[i], teamContours.center[numberTeam]);

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
    robotTeam.resize(3);

    // cropped image around the color team
    for (int i = 0; i < teamContours.center.size(); i++){
        cv::Mat cutImage = image( cv::Rect(teamContours.cutPoint1[i], teamContours.cutPoint2[i]) );

        // search player's color on a cropped image
        for (int j = 0; j < numberOfRobots; j++){
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
    robotOpponent.resize(3);

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
 * Method that receives and analyzes image from webcam
 */
void Vision::imageWebCam(){

    if(cameraOn){
        cam >> opencvImageBGR;
        if (opencvImageBGR.empty()){
            cout << "CRIAR TRATAMENTO DE ERRO PARA IMAGEM VAZIA" << endl;
        }
    }
}

/*
 * Method for initialize webcam
 */
void Vision::initializeWebcam(){

    if(!cameraOn){
        if(cam.isOpened()) cam.release();
        opencvImageBGR = cv::imread(imagePath);

    } else {
        if(!cam.isOpened()) cam = cv::VideoCapture(camera);

        if(cam.isOpened()){
            cam.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
            cam.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
            cam >> opencvImageBGR;

            if(!opencvImageBGR.empty()){
                #ifdef CAMERACONFIG
                    configureCamera();
                #endif
            }
        }
    }
}

/*
 * Setters
 */
void Vision::setCameraRelease(){
    cam.release();
}

void Vision::setRotateField(bool rotate){
    rotateField = int(rotate) * 180;
}

/*
 * Getters
 */
vector<Object> Vision::getRobotTeam(){

    for(int i=0 ; i<robotTeam.size() ; i++){
        if(robotTeam[i].isNull()){
            robotTeam[i] = lastTeam[i];
        } else {
            lastTeam[i] = robotTeam[i];
        }
    }

    return robotTeam;
}

vector<Object> Vision::getOpponent(){
    return robotOpponent;
}

Object Vision::getBall(){

    if(objectBall.isNull()){
        objectBall = lastBall;
    } else {
        lastBall = objectBall;
    }
    
    return objectBall;
}