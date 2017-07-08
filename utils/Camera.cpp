#include "Camera.h"

Camera::Camera(){
    camera_number = getNumberCameraScript();
    camera_comand = getComandCameraScript();
}

void Camera::updateCameraValuesScript(rodetas::CameraConfiguration camera_config){
    
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
            cout << camera_comand[i] << endl;
        }
    }
}

void Camera::defaultCameraScript(){

    string all_comands = rodetas::executeCommand("v4l2-ctl -list | awk ''{ if(NR!=1) {print $1} }'' | grep default | sed 's/default//g'");
    std::stringstream ss(all_comands);
    
    while(ss.good()) {
        std::string comand;
        getline(ss, comand, '\n');
        rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + "--set-ctrl " + comand);
    }
}

vector<string> Camera::getComandCameraScript(){
    string all_comands = rodetas::executeCommand("v4l2-ctl -d /dev/video" + to_string(camera_number) + " -list | awk '{ if(NR!=1) {print $1} }'");
    std::stringstream ss(all_comands);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> v(begin, end);
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    return v;
}

int Camera::getNumberCameraScript(){
    int number = system("utils/./cameraConfig.sh get_camera");
    number = WEXITSTATUS(number);
    return number;
}

int Camera::getNumber(){
    return camera_number;
}