#include "CalibrationModel.h"
#include "../view/CalibrationView.h"

CalibrationModel::CalibrationModel(){
    colorsHSV.resize(6);
    colorsRGB.resize(6);

    selected_player = 0;

    manipulation.loadCalibration();
    goal             = manipulation.getGoal();
    camera_on        = manipulation.getCameraOn();
    colorsHSV        = manipulation.getColorsHsv();
    colorsRGB        = manipulation.getColorsRgb();
    angle_image      = manipulation.getAngleImage();
    camera_config    = manipulation.loadCameraConfig();
    point_cut.first  = manipulation.getPointField1();
    point_cut.second = manipulation.getPointField2();

    hsv_point[0] = colorsHSV[selected_player].h[2];
    hsv_point[1] = colorsHSV[selected_player].s[2];
    hsv_point[2] = colorsHSV[selected_player].v[2];

    opencv_image_BGR = frameInitialize(camera_on);
    frameValidation(opencv_image_BGR, point_cut);
}

CalibrationModel::~CalibrationModel(){
}

bool CalibrationModel::updateFrame(){
    if (camera_on) {
        opencv_image_BGR = cameraUpdate();
    }
    
    if (!opencv_image_BGR.empty()) {
        opencv_image_BGR_rotated = rotateImage( opencv_image_BGR, angle_image);
        opencv_image_BGR_cuted   = cutImage( opencv_image_BGR_rotated, point_cut);
        opencv_image_HSV         = changeColorSpace( opencv_image_BGR_cuted, cv::COLOR_BGR2HSV_FULL);
        opencv_image_cairo       = changeColorSpace( opencv_image_BGR_cuted, cv::COLOR_BGR2RGB);
        opencv_image_binary      = changeColorSpace( binarize(opencv_image_HSV, colorsHSV[selected_player]), cv::COLOR_GRAY2RGB);
        
        caller->notify("updateScreen");

    } else {
        cout << "Empty Image" << endl;
    }

    fps = timer.framesPerSecond();
    
    return true;
}

void CalibrationModel::saveParameters(){
    manipulation.saveCalibration(colorsHSV, colorsRGB, point_cut.first, point_cut.second, goal, angle_image, camera_on);
}

void CalibrationModel::updateColorPixel(Point event_point, Point size_cairo_image){
    Point pixel_point = changeCordinates({event_point.x, event_point.y}, size_cairo_image, opencv_image_BGR_cuted.size());

    hsv_point = opencv_image_HSV.at<cv::Vec3b>(pixel_point.y, pixel_point.x);
    Hsv hsv = colorsHSV[selected_player];
        hsv.setS(hsv_point[S]);
        hsv.setH(hsv_point[H]);
        hsv.setV(hsv_point[V]);
    colorsHSV[selected_player] = hsv;

    rgb_point = opencv_image_BGR_cuted.at<cv::Vec3b>(pixel_point.y, pixel_point.x);
    Rgb rgb = colorsRGB[selected_player];
        rgb.r = rgb_point[2];
        rgb.g = rgb_point[1];
        rgb.b = rgb_point[0];
    colorsRGB[selected_player] = rgb;
    
    caller->notify("defaultHSVPopover");
}

void CalibrationModel::setCaller(CalibrationView* c){
    caller = c;
}

int CalibrationModel::getFps(){
    return fps;
}

cv::Mat CalibrationModel::getScreenImage(){
    return opencv_image_cairo;
}

cv::Mat CalibrationModel::getScreenBinaryImage(){
    return opencv_image_binary;
}