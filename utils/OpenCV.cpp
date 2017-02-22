#include "OpenCV.h"

OpenCV::OpenCV(){
    camera_number = getCameraNumber();
    cam.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
}

/*
 * Method that binarize the image with opencv
 */
cv::Mat OpenCV::opencvBinary(Hsv color, cv::Mat image){

    cv::inRange(image, cv::Scalar(color.h[0], color.s[0], color.v[0]), cv::Scalar(color.h[1], color.s[1], color.v[1]), image); 
    cv::medianBlur(image, image, 3);

    return image;
}

/*
 * Method to convert color space
 */
cv::Mat OpenCV::opencvColorSpace(cv::Mat image, int code){

    cv::Mat newColorSpace;
    cv::cvtColor(image, newColorSpace, code);

    return newColorSpace;
}

/*
 * Method that cuts, rotates and resize
 */
cv::Mat OpenCV::opencvTransformation(cv::Mat image, int angle, cv::Point point_cut_field_1, cv::Point point_cut_field_2){

    cv::Mat imageRotated;
    cv::warpAffine( image, imageRotated, cv::getRotationMatrix2D( cv::Point(image.cols/2, image.rows/2), angle - 180, 1), imageRotated.size() );
    cv::Mat cutImage = imageRotated( cv::Rect(point_cut_field_1, point_cut_field_2) );

    return cutImage;
}

/*
 * Method for find contours in binarized image
 */
BlobsContours OpenCV::blobContour(cv::Mat image, float blobSize){

    BlobsContours blobContours;
    
    vector< vector<cv::Point> > contours;
    vector<cv::Vec4i>           hierarchy;

    cv::findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    vector< float>              radius (contours.size());
    vector< cv::Point2f>        center (contours.size());    
    vector< vector<cv::Point> > contours_poly (contours.size());
    
    for(int i = 0; i < contours.size(); i++){
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        cv::minEnclosingCircle(contours_poly[i], center[i], radius[i] );
    
        if (radius[i] > blobSize * 0.7){

            Point2f cutPoint1 = cv::Point( center[i].x - radius[i], center[i].y - radius[i] );
            Point2f cutPoint2 = cv::Point( center[i].x + radius[i], center[i].y + radius[i] );

            if (cutPoint1.x < 0) cutPoint1.x = 0;
            if (cutPoint1.y < 0) cutPoint1.y = 0;
            if (cutPoint2.x < 0) cutPoint2.x = 0;
            if (cutPoint2.y < 0) cutPoint2.y = 0;
            if (cutPoint1.x > image.cols) cutPoint1.x = image.cols;
            if (cutPoint1.y > image.rows) cutPoint1.y = image.rows;
            if (cutPoint2.x > image.cols) cutPoint2.x = image.cols;
            if (cutPoint2.y > image.rows) cutPoint2.y = image.rows;

            blobContours.center.push_back(center[i]);
            blobContours.radius.push_back(radius[i]);
            blobContours.cutPoint1.push_back(cutPoint1);
            blobContours.cutPoint2.push_back(cutPoint2);
        }
    }

    return blobContours;
}

float OpenCV::blobRadius(cv::Mat image){

    float blobRadius = 0;
    
    vector< vector<cv::Point> > contours;
    vector<cv::Vec4i>           hierarchy;

    cv::findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    vector< float>              radius (contours.size());
    vector< cv::Point2f>        center (contours.size());    
    vector< vector<cv::Point> > contours_poly (contours.size());
    
    for(int i = 0; i < contours.size(); i++){
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        cv::minEnclosingCircle(contours_poly[i], center[i], radius[i] );
        
        if (blobRadius < radius[i]){
            blobRadius = radius[i];
        }

    }

    return blobRadius;
}

/*
 * Method for initialize image
 */
void OpenCV::imageInitialize(bool camera_on){

    if(camera_on){
        cam = cv::VideoCapture(camera_number);
        timer.wait(100000); //time to camera answer
        if(cam.isOpened()){
            cam >> opencv_image_BGR;
            updateCameraValues(camera_config, camera_number);
        } else {
            cout << "CONECTION WITH CAMERA FAILED" << endl;
            camera_on = false;  
        }
    } 
    
    if(!camera_on) {
        setCameraRelease();

        opencv_image_BGR = cv::imread(imagePath);

        if(opencv_image_BGR.empty()){
            cout << "PROBLEM TO LOAD IMAGE FROM COMPUTER" << endl;
        }
    }

    if (point_cut_field_1.x > opencv_image_BGR.rows || point_cut_field_2.x > opencv_image_BGR.rows ||
        point_cut_field_1.y > opencv_image_BGR.cols || point_cut_field_2.y > opencv_image_BGR.cols) {
            point_cut_field_1 = {0,0};
            point_cut_field_2 = opencv_image_BGR.size();
    }
}

/*
 * Method that receives and analyzes image from webcam
 */
void OpenCV::imageWebCam(bool camera_on){
    if(camera_on && cam.isOpened()){
        cam >> opencv_image_BGR;
    }
}

void OpenCV::setCameraRelease(){
    if(cam.isOpened()){
        cam.release();
    }
}