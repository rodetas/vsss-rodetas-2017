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

    float percent_cut = 30;
    
    if (!last_position.review_all_image && last_position.cutPointDefined()) {

        for(int j = 0; j < last_position.center.size(); j++) {

            cv::Mat image_cut = cutImage(image, last_position.cutPoint[j]);
                    image_cut = changeColorSpace(image_cut, cv::COLOR_BGR2HSV_FULL);
                    image_cut = binarize(image_cut, color_hsv);

            find_position = binarizedColorPosition(image_cut);

            for(int i = 0; i < find_position.center.size(); i++){
                PointCut cutPoint;
                cutPoint.first = cv::Point( find_position.center[i].x - (percent_cut) + last_position.cutPoint[j].first.x, find_position.center[i].y - (percent_cut) + last_position.cutPoint[j].first.y);
                cutPoint.second = cv::Point( find_position.center[i].x + (percent_cut) + last_position.cutPoint[j].first.x, find_position.center[i].y + (percent_cut) + last_position.cutPoint[j].first.y);

                Point2i center_position = cv::Point( find_position.center[i].x + last_position.cutPoint[j].first.x, find_position.center[i].y + last_position.cutPoint[j].first.y);

                atual_position.center.push_back(center_position);
                atual_position.radius.push_back(find_position.radius[i]);
                atual_position.cutPoint.push_back(cutPoint);

                atual_position.verifyLimits(image.cols, image.rows);
            }
        }
        
    } else if (last_position.review_all_image) {

        cv::Mat binary_image   = changeColorSpace(image, cv::COLOR_BGR2HSV_FULL);
                binary_image   = binarize(binary_image, color_hsv);    

        find_position = binarizedColorPosition(binary_image, n_contours);

        for(int i = 0; i < find_position.center.size(); i++){
            PointCut cutPoint;            
            cutPoint.first = cv::Point( find_position.center[i].x - (percent_cut), find_position.center[i].y - (percent_cut));
            cutPoint.second = cv::Point( find_position.center[i].x + (percent_cut), find_position.center[i].y + (percent_cut));

            atual_position.center.push_back(find_position.center[i]);
            atual_position.radius.push_back(find_position.radius[i]);
            atual_position.cutPoint.push_back(cutPoint);
            
            atual_position.verifyLimits(image.cols, image.rows);
        }
    }
    
    atual_position.reviewAllImage(last_position.frames);

    return atual_position;
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

    //do {
        cout << "AQUI" << endl;
        camera.updateCameraValuesScript(camera_config);
        cout << "AQUI2" << endl;
        
        cam = cv::VideoCapture(camera.getNumber());
        timer.wait(500000); //time to camera answer

        if(cam.isOpened()){
            cam.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
            cam.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
            cam >> image;
        } else {
            cout << "CONECTION WITH CAMERA FAILED" << endl;
        }
    //} while(!cam.isOpened());

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