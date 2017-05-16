#include "OpenCV.h"

OpenCV::OpenCV(){
    camera_number = getCameraNumber();
}

/*
 * Method that binarize the image with opencv
 */
cv::Mat OpenCV::binarize(cv::Mat image, Hsv color){

    cv::inRange(image, cv::Scalar(color.h[0], color.s[0], color.v[0]), cv::Scalar(color.h[1], color.s[1], color.v[1]), image); 
    cv::medianBlur(image, image, 3);

    return image;
}

/*
 * Method to convert color space
 */
cv::Mat OpenCV::changeColorSpace(cv::Mat image, int code){

    cv::Mat newColorSpace;
    cv::cvtColor(image, newColorSpace, code);

    return newColorSpace;
}

/*
 * Method that cut image
 */
cv::Mat OpenCV::cutImage(cv::Mat image, cv::Point point_cut_field_1, cv::Point point_cut_field_2){
    return image( cv::Rect(point_cut_field_1, point_cut_field_2) );
}

/*
 * Method that rotate image
 */
cv::Mat OpenCV::rotateImage(cv::Mat image, int angle){
    cv::Mat imageRotated;
    cv::warpAffine( image, imageRotated, cv::getRotationMatrix2D( cv::Point(image.cols/2, image.rows/2), angle - 180, 1), image.size());
    return imageRotated;
}

/*
 * Method for find contours in binarized image
 */
ContoursPosition OpenCV::binarizedColorPosition(cv::Mat image, int n_contours = 1){

    ContoursPosition position;
 
    vector< vector<cv::Point> > contours;
    vector<cv::Vec4i>           hierarchy;

    cv::findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    
    sort(contours.begin(), contours.end(), 
        [](const vector<Point> c1, const vector<Point> c2){
            return cv::contourArea(c1, false) < cv::contourArea(c2, false);
        }); 
    
    if (contours.size() < n_contours)
        n_contours = contours.size();
    
    vector<float>              radius (n_contours);
    vector<cv::Point2f>        center (n_contours);    
    vector<vector<cv::Point> > contours_poly (n_contours);

    for (int i = 0; i < n_contours; i++){
        cv::approxPolyDP( cv::Mat(contours[contours.size() - i - 1]), contours_poly[i], 3, true );
        cv::minEnclosingCircle(contours_poly[i], center[i], radius[i] );

        position.center.push_back(center[i]);
        position.radius.push_back(radius[i]);
    }

    return position;
}


// cv::imshow("T", image_cut);   cv::waitKey();
ContoursPosition OpenCV::position(cv::Mat image, ContoursPosition last_position, Hsv color_hsv, int n_contours){

    ContoursPosition atual_position, find_position;

    percent_cut = 40;
    
    
    if (!last_position.review_all_image && last_position.cutPointDefined()) {

        for(int j = 0; j < last_position.center.size(); j++) {

            cv::Mat image_cut = cutImage(image, last_position.cutPoint1[j], last_position.cutPoint2[j]);
                    image_cut = changeColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                    image_cut = binarize(image_cut, color_hsv);


            find_position = binarizedColorPosition(image_cut);

            for(int i = 0; i < find_position.center.size(); i++){
                Point2i cutPoint1 = cv::Point( find_position.center[i].x - (percent_cut) + last_position.cutPoint1[j].x, find_position.center[i].y - (percent_cut) + last_position.cutPoint1[j].y);
                Point2i cutPoint2 = cv::Point( find_position.center[i].x + (percent_cut) + last_position.cutPoint1[j].x, find_position.center[i].y + (percent_cut) + last_position.cutPoint1[j].y);
                Point2i center_position = cv::Point( find_position.center[i].x + last_position.cutPoint1[j].x, find_position.center[i].y + last_position.cutPoint1[j].y);

                atual_position.center.push_back(center_position);
                atual_position.radius.push_back(find_position.radius[i]);
                atual_position.cutPoint1.push_back(cutPoint1);
                atual_position.cutPoint2.push_back(cutPoint2);

                atual_position.verifyLimits(image.cols, image.rows);
            }
        }
        
    } else if (last_position.review_all_image) {

        cv::Mat binary_image   = changeColorSpace(image, cv::COLOR_BGR2HSV_FULL);
                binary_image   = binarize(binary_image, color_hsv);    

        find_position = binarizedColorPosition(binary_image, n_contours);

        for(int i = 0; i < find_position.center.size(); i++){
            Point2i cutPoint1 = cv::Point( find_position.center[i].x - (percent_cut), find_position.center[i].y - (percent_cut));
            Point2i cutPoint2 = cv::Point( find_position.center[i].x + (percent_cut), find_position.center[i].y + (percent_cut));

            atual_position.center.push_back(find_position.center[i]);
            atual_position.radius.push_back(find_position.radius[i]);
            atual_position.cutPoint1.push_back(cutPoint1);
            atual_position.cutPoint2.push_back(cutPoint2);
            
            atual_position.verifyLimits(image.cols, image.rows);
        }
    }
    
    atual_position.reviewAllImage(last_position.frames);

    return atual_position;
}

/*
 * Method that receives image from webcam
 */
void OpenCV::setImage(){    
    if (image_initialize) 
        imageInitialize();
    
    if (camera_initialize)
        cameraInitialize();

    if(camera_on && cam.isOpened())
        cam >> opencv_image_BGR;
}

/*
 * Method for initialize image
 */
void OpenCV::imageInitialize(){
    cameraRelease();
    do {        
        opencv_image_BGR = cv::imread(imagePath);
        
        if (opencv_image_BGR.empty()){
            cout << "PROBLEM TO LOAD IMAGE FROM COMPUTER" << endl;
        }

    } while(opencv_image_BGR.empty());
    
    image_initialize = false;
    imageValidation();
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
    camera_initialize = false;
    imageValidation();
}

/*
 * Method to verify the image integrity
 */
void OpenCV::imageValidation(){
    bool validate = false;
    while (!validate){

        if (opencv_image_BGR.empty()){
            cout << "EMPTY IMAGE" << endl;

        } else if ( point_cut_field_1.x > opencv_image_BGR.cols || point_cut_field_1.y > opencv_image_BGR.rows ||
                    point_cut_field_2.x > opencv_image_BGR.cols || point_cut_field_2.y > opencv_image_BGR.rows) {
            point_cut_field_1 = {0,0};
            point_cut_field_2 = opencv_image_BGR.size();
            cout << "POINT TO CUT IMAGE IS INVALID" << endl;

        } else {
            validate = true;
            cout << "IMAGE IS VALID" << endl;
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