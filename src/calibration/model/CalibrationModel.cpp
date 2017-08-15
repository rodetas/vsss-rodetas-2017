#include "CalibrationModel.h"
#include "../view/CalibrationView.h"

CalibrationModel::CalibrationModel(){
    colorsHSV.resize(6);
    colorsRGB.resize(6);

    selected_player = 0;

    manipulation.loadCalibration();
    point_goal       = manipulation.getGoal();
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

    updateDevice();
    camera.setCameraValuesScript(camera_config);

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
    
    caller->notify("setScaleValueHSVDefault");
}

void CalibrationModel::updateDevice(){
    string device;

    device = executeCommand("uvcdynctrl -l | grep video0 | sed 's/.*video0   //'");
    if(device.size() != 0){
        device.erase(std::remove(device.begin(), device.end(), '\n'), device.end());
        name_device0 = device;
    } else {
        name_device0 = "Device 0 not found";        
    }

    device = executeCommand("uvcdynctrl -l | grep video1 | sed 's/.*video1   //'");
    if(device.size() != 0){
        device.erase(std::remove(device.begin(), device.end(), '\n'), device.end());        
        name_device1 = device;
    } else {
        name_device1 = "Device 1 not found";
    }
}

void CalibrationModel::saveParameters(){
    manipulation.saveCalibration(colorsHSV, colorsRGB, point_cut.first, point_cut.second, point_goal, angle_image, camera_on);
    manipulation.saveCameraConfig(camera_config);
}

void CalibrationModel::setCaller(CalibrationView* c){
    caller = c;
}

void CalibrationModel::setCutPoint(PointCut cut, Point cairo_size){
    point_cut.first = changeCordinates(cut.first, cairo_size, opencv_image_BGR.size());
    point_cut.second = changeCordinates(cut.second, cairo_size, opencv_image_BGR.size());
}

void CalibrationModel::setCutGoal(PointCut cut, Point cairo_size){
    Point p1 = changeCordinates(cut.first, cairo_size, opencv_image_BGR.size());
    Point p2 = changeCordinates(cut.second, cairo_size, opencv_image_BGR.size());
    point_goal = {abs(p1.x - p2.x), abs(p1.y - p2.y)};
}

void CalibrationModel::setAngleImage(double d){
    angle_image = d;
}

void CalibrationModel::setCameraOn(bool b){
    camera_on = b;
    opencv_image_BGR = frameInitialize(camera_on);
}

void CalibrationModel::setSelectedPlayer(int i){
    selected_player = i;
    
    caller->notify("setScaleValueHSV");
}

void CalibrationModel::setScaleHSV(string s, double value){
    if (s == "hmax"){
        colorsHSV[selected_player].variationH_MAX = value;
        colorsHSV[selected_player].setH(colorsHSV[selected_player].h[MID]);

    } else if (s == "hmin"){
        colorsHSV[selected_player].variationH_MIN = value;
        colorsHSV[selected_player].setH(colorsHSV[selected_player].h[MID]);

    } else if (s == "smax"){
        colorsHSV[selected_player].variationS_MAX = value;
        colorsHSV[selected_player].setS(colorsHSV[selected_player].s[MID]);
        
    } else if (s == "smin"){
        colorsHSV[selected_player].variationS_MIN = value;
        colorsHSV[selected_player].setS(colorsHSV[selected_player].s[MID]);
        
    } else if (s == "vmax"){
        colorsHSV[selected_player].variationV_MAX = value;
        colorsHSV[selected_player].setV(colorsHSV[selected_player].v[MID]);
        
    } else if (s == "vmin"){
        colorsHSV[selected_player].variationV_MIN = value;
        colorsHSV[selected_player].setV(colorsHSV[selected_player].v[MID]);
    }
}

void CalibrationModel::setScaleCam(string s, double value){
    if (s == "brightness") {
        camera_config.brightness = value;

    } else if (s == "contrast") {
        camera_config.contrast = value;

    } else if (s == "saturation") {
        camera_config.saturation = value;

    } else if (s == "gain") {
        camera_config.gain = value;

    } else if (s == "sharpness") {
        camera_config.sharpness = value;

    } else if (s == "exposure") {
        camera_config.exposure = value;
    }

    camera.setCameraValuesScript(camera_config);
}

int CalibrationModel::getFps(){
    return fps;
}

bool CalibrationModel::getCameraOn(){
    return camera_on;
}

Hsv CalibrationModel::getColorHsv(){
    return colorsHSV[selected_player];
}

cv::Mat CalibrationModel::getScreenImage(){
    return opencv_image_cairo;
}

cv::Mat CalibrationModel::getScreenBinaryImage(){
    return opencv_image_binary;
}

string CalibrationModel::getDeviceName(int n){
    if (n == 0)
        return name_device0;
    else
        return name_device1;
}

CameraConfiguration CalibrationModel::getDefaultCameraValues(){
    camera.defaultCameraScript();
    camera_config = camera.getCameraValuesScript();
    return camera_config;
}