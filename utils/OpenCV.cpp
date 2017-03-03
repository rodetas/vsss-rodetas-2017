#include "OpenCV.h"

OpenCV::OpenCV(){
    camera_number = getCameraNumber();
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
ContoursPosition OpenCV::findPosition(cv::Mat image, int n_contours = 1){

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

// fazer algo para se momentaneamente perder algo por conta da velocidade
// verificar tamanho padrão
// cv::imshow("T", image_cut);   cv::waitKey();

ContoursPosition OpenCV::position(cv::Mat image, ContoursPosition last_position, Hsv color_hsv, int n_contours){

    ContoursPosition atual_position, find_position;

    if (!last_position.review_all_image && last_position.cutPointDefined()) {

        for(int j = 0; j < last_position.center.size(); j++) {

            cv::Mat full_image_cut  = opencvCutImage(image, point_cut_field_1, point_cut_field_2);
            cv::Mat image_cut       = opencvCutImage(full_image_cut, last_position.cutPoint1[j], last_position.cutPoint2[j]);
                    image_cut       = opencvRotateImage(image_cut, angle_image);
                    image_cut       = opencvColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                    image_cut       = opencvBinary(image_cut, color_hsv);
            
            find_position = findPosition(image_cut);

            for(int i = 0; i < find_position.center.size(); i++){

                Point2f cutPoint1 = cv::Point( (find_position.center[i].x - find_position.radius[i] + last_position.cutPoint1[j].x)  * 0.95, (find_position.center[i].y - find_position.radius[i] + last_position.cutPoint1[j].y)  * 0.95);
                Point2f cutPoint2 = cv::Point( (find_position.center[i].x + find_position.radius[i] + last_position.cutPoint1[j].x)  * 1.05, (find_position.center[i].y + find_position.radius[i] + last_position.cutPoint1[j].y)  * 1.05);
                Point2f center_position = cv::Point( find_position.center[i].x + last_position.cutPoint1[j].x, find_position.center[i].y + last_position.cutPoint1[j].y);
                
                if (cutPoint1.x < 0) cutPoint1.x = 0;
                if (cutPoint1.y < 0) cutPoint1.y = 0;
                if (cutPoint2.x < 0) cutPoint2.x = 0;
                if (cutPoint2.y < 0) cutPoint2.y = 0;
                if (cutPoint1.x > full_image_cut.cols) cutPoint1.x = full_image_cut.cols;
                if (cutPoint1.y > full_image_cut.rows) cutPoint1.y = full_image_cut.rows;
                if (cutPoint2.x > full_image_cut.cols) cutPoint2.x = full_image_cut.cols;
                if (cutPoint2.y > full_image_cut.rows) cutPoint2.y = full_image_cut.rows;
                if (center_position.x > full_image_cut.cols) center_position.x = full_image_cut.cols;
                if (center_position.y > full_image_cut.rows) center_position.y = full_image_cut.rows;

                atual_position.center.push_back(center_position);
                atual_position.radius.push_back(find_position.radius[i]);
                atual_position.cutPoint1.push_back(cutPoint1);
                atual_position.cutPoint2.push_back(cutPoint2);
            }
        }
        
    } else if (last_position.review_all_image) {

        cv::Mat binary_image = opencvRotateImage(image, angle_image);
                binary_image = opencvCutImage(binary_image, point_cut_field_1, point_cut_field_2);
                binary_image = opencvColorSpace(binary_image, cv::COLOR_BGR2HSV_FULL);
                binary_image = opencvBinary(binary_image, color_hsv);

        find_position = findPosition(binary_image, n_contours);

        for(int i = 0; i < find_position.center.size(); i++){

            Point2f cutPoint1 = cv::Point( (find_position.center[i].x - find_position.radius[i]) * 0.95, (find_position.center[i].y - find_position.radius[i]) * 0.95 );
            Point2f cutPoint2 = cv::Point( (find_position.center[i].x + find_position.radius[i]) * 1.05, (find_position.center[i].y + find_position.radius[i]) * 1.05 );

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