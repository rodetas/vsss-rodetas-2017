#include "OpenCV.h"

OpenCV::OpenCV(){
    camera_number = getCameraNumber();
    review_all_image = false;
}

/*
 * Method that binarize the image with opencv
 */
cv::Mat OpenCV::opencvBinary(cv::Mat image, Hsv color){

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
 * Method that cut image
 */
cv::Mat OpenCV::opencvCutImage(cv::Mat image, cv::Point point_cut_field_1, cv::Point point_cut_field_2){
    return image( cv::Rect(point_cut_field_1, point_cut_field_2) );
}

/*
 * Method that rotate image
 */
cv::Mat OpenCV::opencvRotateImage(cv::Mat image, int angle){
    cv::Mat imageRotated;
    cv::warpAffine( image, imageRotated, cv::getRotationMatrix2D( cv::Point(image.cols/2, image.rows/2), angle - 180, 1), imageRotated.size() );
    return imageRotated;
}

/*
 * Method for find contours in binarized image
 */
ContoursPosition OpenCV::findPosition(cv::Mat image){

    ContoursPosition position;
 
    vector< vector<cv::Point> > contours;
    vector<cv::Vec4i>           hierarchy;

    cv::findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    vector< float>              radius (contours.size());
    vector< cv::Point2f>        center (contours.size());    
    vector< vector<cv::Point> > contours_poly (contours.size());
    
    for(int i = 0; i < contours.size(); i++){
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        cv::minEnclosingCircle(contours_poly[i], center[i], radius[i] );

        position.center.push_back(center[i]);
        position.radius.push_back(radius[i]);
    }

    return position;
}

ContoursPosition OpenCV::position(cv::Mat image, ContoursPosition last_position, Hsv color_hsv){

    ContoursPosition atual_position, find_position;

    if (last_position.cutPointDefined() && review_all_image) {

        for(int j = 0; j < last_position.center.size(); j++) {
            
            cv::Mat image_cut = opencvCutImage(image, last_position.cutPoint1[j], last_position.cutPoint2[j]);
                    image_cut = opencvRotateImage(image_cut, angle_image);
                    image_cut = opencvColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                    image_cut = opencvBinary(image_cut, color_hsv);

            find_position = findPosition(image_cut);
            
            for(int i = 0; i < find_position.center.size(); i++){

                Point2f cutPoint1 = cv::Point( (find_position.center[i].x - find_position.radius[i] + last_position.cutPoint1[j].x)  * 0.9, (find_position.center[i].y - find_position.radius[i] + last_position.cutPoint1[j].y)  * 0.9);
                Point2f cutPoint2 = cv::Point( (find_position.center[i].x + find_position.radius[i] + last_position.cutPoint1[j].x)  * 1.1, (find_position.center[i].y + find_position.radius[i] + last_position.cutPoint1[j].y)  * 1.1);
                Point2f center_position = cv::Point( find_position.center[i].x + last_position.cutPoint1[j].x, find_position.center[i].y + last_position.cutPoint1[j].y);
                
                if (cutPoint1.x < 0) cutPoint1.x = 0;
                if (cutPoint1.y < 0) cutPoint1.y = 0;
                if (cutPoint2.x < 0) cutPoint2.x = 0;
                if (cutPoint2.y < 0) cutPoint2.y = 0;
                if (cutPoint1.x > image.cols) cutPoint1.x = image.cols;
                if (cutPoint1.y > image.rows) cutPoint1.y = image.rows;
                if (cutPoint2.x > image.cols) cutPoint2.x = image.cols;
                if (cutPoint2.y > image.rows) cutPoint2.y = image.rows;
                if (center_position.x > image.cols) center_position.x = image.cols;
                if (center_position.y > image.rows) center_position.y = image.rows;

                atual_position.center.push_back(center_position);
                atual_position.radius.push_back(find_position.radius[i]);
                atual_position.cutPoint1.push_back(cutPoint1);
                atual_position.cutPoint2.push_back(cutPoint2);

                if (center_position.x == 0 || center_position.y == 0){
                    review_all_image = false;
                }
            }
        }
        
    } else {

        cv::Mat binary_image = opencvRotateImage(image, angle_image);
                binary_image = opencvCutImage(binary_image, point_cut_field_1, point_cut_field_2);
                binary_image = opencvColorSpace(binary_image, cv::COLOR_BGR2HSV_FULL);
                binary_image = opencvBinary(binary_image, color_hsv);

            find_position = findPosition(binary_image);
            
            for(int i = 0; i < find_position.center.size(); i++){

                Point2f cutPoint1 = cv::Point( (find_position.center[i].x - find_position.radius[i]) * 0.9, (find_position.center[i].y - find_position.radius[i]) * 0.9 );
                Point2f cutPoint2 = cv::Point( (find_position.center[i].x + find_position.radius[i]) * 1.1, (find_position.center[i].y + find_position.radius[i]) * 1.1 );

                if (cutPoint1.x < 0) cutPoint1.x = 0;
                if (cutPoint1.y < 0) cutPoint1.y = 0;
                if (cutPoint2.x < 0) cutPoint2.x = 0;
                if (cutPoint2.y < 0) cutPoint2.y = 0;
                if (cutPoint1.x > binary_image.cols) cutPoint1.x = binary_image.cols;
                if (cutPoint1.y > binary_image.rows) cutPoint1.y = binary_image.rows;
                if (cutPoint2.x > binary_image.cols) cutPoint2.x = binary_image.cols;
                if (cutPoint2.y > binary_image.rows) cutPoint2.y = binary_image.rows;

                atual_position.center.push_back(find_position.center[i]);
                atual_position.radius.push_back(find_position.radius[i]);
                atual_position.cutPoint1.push_back(cutPoint1);
                atual_position.cutPoint2.push_back(cutPoint2);

                review_all_image = true;
        }
    }

    //cout << "Size: " << find_position.center.size() << endl;

    return atual_position;
}

/*
 * Method that receives image from webcam
 */
void OpenCV::imageWebCam(){
    if(camera_on && cam.isOpened()){
        cam >> opencv_image_BGR;
    }
}

/*
 * Method for initialize image
 */
void OpenCV::imageInitialize(){
    cameraRelease();
    do {
        if (!opencv_image_BGR.empty()){
            opencv_image_BGR = cv::imread(imagePath);
        } else {
            cout << "PROBLEM TO LOAD IMAGE FROM COMPUTER" << endl;
        }
    } while(opencv_image_BGR.empty());
}


/*
 * Method to initialize camera
 */
void OpenCV::cameraInitialize(){
    while(!cam.isOpened()){
        updateCameraValues(camera_config, camera_number);
        cam = cv::VideoCapture(camera_number);
        timer.wait(500000); //time to camera answer

        if(cam.isOpened()){
            cam.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
            cam.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
            cam >> opencv_image_BGR;            
        } else {
            cout << "CONECTION WITH CAMERA FAILED" << endl;
        }  
    }
}

/*
 * Method to disconnect camera
 */
void OpenCV::cameraRelease(){
    if(cam.isOpened()){
        cam.release();
    }
}