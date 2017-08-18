#include "Camera.h"

Camera::Camera(){
    camera_number = getNumberCameraScript();
    camera_comand = getComandCameraScript();
}

void Camera::setCameraValuesScript(rodetas::CameraConfiguration camera_config){
    
    for (int i = 0; i < camera_comand.size(); i++){
        
        if (camera_comand[i] == "brightness") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl brightness=" + to_string(camera_config.brightness));
        }
        else if (camera_comand[i] == "contrast") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl contrast=" + to_string(camera_config.contrast));
        }
        else if (camera_comand[i] == "saturation") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl saturation=" + to_string(camera_config.saturation));
        }
        else if (camera_comand[i] == "gain") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl gain=" + to_string(camera_config.gain));
        }
        else if (camera_comand[i] == "sharpness") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl sharpness=" + to_string(camera_config.sharpness));
        }
        else if (camera_comand[i] == "exposure_absolute") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl exposure_absolute=" + to_string(camera_config.exposure));
        }
        else if (camera_comand[i] == "focus_auto") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl focus_auto=0 ");
        }
        else if (camera_comand[i] == "exposure_auto") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl exposure_auto=1 ");
        }
        else if (camera_comand[i] == "backlight_compensation") {
            rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl backlight_compensation=0");
        }
        else {
            //  cout << "COMAND NOT FOUND: " << camera_comand[i] << endl;
        }
    }
}

rodetas::CameraConfiguration Camera::getCameraValuesScript(){

    rodetas::CameraConfiguration camera_config;
    
    vector<string> values;
    string all_comands = rodetas::executeCommand("v4l2-ctl -list | sed 's/.*value=//' | awk '{ if(NR!=1) {print $1} }'");

    std::stringstream stream(all_comands);
    std::string line;    
    while (std::getline(stream, line)) {
        values.push_back(line);
    }

    for (int i = 0; i < camera_comand.size(); i++){

        if (camera_comand[i] == "brightness") {
            camera_config.brightness = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "contrast") {
            camera_config.contrast = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "saturation") {
            camera_config.saturation = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "gain") {
            camera_config.gain = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "sharpness") {
            camera_config.sharpness = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "exposure_absolute") {
            camera_config.exposure = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "focus_auto") {
            //camera_config.focus_auto = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "exposure_auto") {
            //camera_config.exposure_auto = std::stoi(values[i]);
        }
        else if (camera_comand[i] == "backlight_compensation") {
            //camera_config.backlight_compensation = std::stoi(values[i]);
        }
        else {
            //  cout << "COMAND NOT FOUND: " << camera_comand[i] << endl;
        }
    }
    return camera_config;    
}

void Camera::defaultCameraScript(){
    vector<string> values_default;
    string all_comands = rodetas::executeCommand("v4l2-ctl -list | sed 's/.*default=//' | awk '{ if(NR!=1) {print $1} }'");

    std::stringstream stream(all_comands);
    std::string line;    
    while (std::getline(stream, line)) {
        values_default.push_back(line);
    }
    
    for (int i = 0; i < values_default.size(); i++){
        rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " --set-ctrl " + camera_comand[i] + "=" + values_default[i]);
    }
}

vector<string> Camera::getComandCameraScript(){
    vector<string> v;
    string all_comands = rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " -list | awk '{ if(NR!=1) {print $1} }'");

    std::stringstream stream(all_comands);
    std::string line;
    while (std::getline(stream, line)) {
        v.push_back(line);
    }   

    return v;
}

int Camera::getNumberCameraScript(){
    int number = system("utils/./shell/cameraConfig.sh get_camera");
    number = WEXITSTATUS(number);
    return number;
}

int Camera::getNumber(){
    return camera_number;
}