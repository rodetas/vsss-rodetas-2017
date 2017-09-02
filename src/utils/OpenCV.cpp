#include "OpenCV.h"

OpenCV::OpenCV(){
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
cv::Mat OpenCV::cutImage(cv::Mat image, PointCut point){

    /*cout << "size " << image.cols << " " << image.rows << endl;
    cout << "point " << point.first << " " << point.second << endl << endl;*/

    if(point.second.y > image.rows) point.second.y = image.rows;
    if(point.second.y < 0) point.second.y = 0;
    if(point.second.x > image.cols) point.second.x = image.cols;
    if(point.first.x < 0) point.first.x = 0;
    if(point.first.y < 0) point.first.y = 0;

    return image( cv::Rect(point.first, point.second) );
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
Position OpenCV::binarizedColorPosition(cv::Mat image, int n_contours = 1){

    Position position;
 
    vector< vector<cv::Point> > contours;
    vector<cv::Vec4i>           hierarchy;

    cv::findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // sort in crescent order the contours vector by found area
    sort(contours.begin(), contours.end(), 
        [](const vector<Point> c1, const vector<Point> c2){
            return cv::contourArea(c1, false) < cv::contourArea(c2, false);
        });
    
    if (contours.size() < n_contours)
        n_contours = contours.size();       
    
    vector<float>       radius (n_contours);
    vector<cv::Point2f> center (n_contours);
    
    // to take the biggest contours (contours.size() - 1)
    for (int i = 0; i < n_contours; i++){
        cv::minEnclosingCircle(contours[(contours.size() - 1) - i], center[i], radius[i] );

        position.center.push_back(center[i]);
        position.radius.push_back(radius[i]);
    }

    return position;
}

Position OpenCV::position(cv::Mat image, Hsv color_hsv, int n_contours) {
    cv::Mat binary_image = changeColorSpace(image, cv::COLOR_BGR2HSV_FULL);
            binary_image = binarize(binary_image, color_hsv);
    
    Position find_position = binarizedColorPosition(binary_image, n_contours);
    
    return find_position;
}

Position OpenCV::position(cv::Mat image, Hsv color_hsv, int n_contours, PointCut point){
    cv::Mat image_cut = cutImage(image, point);
            image_cut = changeColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
            image_cut = binarize(image_cut, color_hsv);
    
    Position find_position = binarizedColorPosition(image_cut, n_contours);

    for(int i = 0; i < find_position.size(); i++){
        find_position.changeCoordinateToGlobal(i, point);
    }
    
    return find_position;
}

/*
 * Method to verify the image integrity
 */
void OpenCV::frameValidation(cv::Mat image, PointCut point){
    do {
        if (image.empty()){
            cout << "EMPTY IMAGE" << endl;

        } else if ( point.first.x > image.cols || point.first.y > image.rows ||
                    point.second.x > image.cols || point.second.y > image.rows) {
            point.first = {0,0};
            point.second = image.size();
            cout << "POINT TO CUT IMAGE IS INVALID" << endl;

        } else {
            cout << "IMAGE IS VALID" << endl;
            break;
        }
    } while(true);
}

/*
 * Method for initialize image
 */
cv::Mat OpenCV::imageInitialize(){
    cv::Mat image;
    cameraRelease();

    do {
        image = cv::imread("../files/images/1280x720.png");        
        if (image.empty()){
            cout << "PROBLEM TO LOAD IMAGE FROM COMPUTER" << endl; 
        }
    } while(image.empty());

    return image;
}

/*
 * Method to initialize camera
 */
cv::Mat OpenCV::cameraInitialize(){
    cv::Mat image;

    do {        
        cam = cv::VideoCapture(camera.getNumber());
        timer.wait(500000); //time to camera answer

        if(cam.isOpened()){
            cam.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
            cam.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
            cam >> image;
        } else {
            cout << "CONECTION WITH CAMERA FAILED" << endl;
        }
    } while(!cam.isOpened());

    return image;
}

/*
 * Method that receives image from webcam
 */
cv::Mat OpenCV::cameraUpdate(){
    cv::Mat image;

    if(cam.isOpened())
        cam >> image;

    return image;
}

/*
 * Method to disconnect camera
 */
void OpenCV::cameraRelease(){
    if(cam.isOpened()){
        cam.release();
    }
}

cv::Mat OpenCV::frameInitialize(bool camera_on){
    cv::Mat image;
    if (camera_on) {
        image = cameraInitialize();
    } else {
        image = imageInitialize();
    }

    return image;
}