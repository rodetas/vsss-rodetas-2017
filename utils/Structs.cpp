#include "Structs.h"

namespace rodetas{

    int getCameraNumber(){
        int cameraNumber = system("utils/./cameraConfig.sh get_camera");
        cameraNumber = WEXITSTATUS(cameraNumber);

        return cameraNumber;
    }

    string executeCommand(string cmd){

        char buf[256];
        string output = "";
        FILE *fp = popen(cmd.c_str(), "r");

        while(fgets(buf, 256, fp) != NULL) {
            // Do whatever you want here...
            output += buf;    
        }

        pclose(fp);

        return output;
    }

    Point changeCordinates(Point point_before, Point size_before, Point size_after){
        Point point_after;
            point_after.x = float(point_before.x) * float(size_after.x) / float(size_before.x);
            point_after.y = float(point_before.y) * float(size_after.y) / float(size_before.y);
        return point_after;
    }

    void updateCameraValues(CameraConfiguration camera_config, int camera){
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Brightness' " + to_string(camera_config.brightness));
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Contrast' " + to_string(camera_config.contrast));
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Saturation' " + to_string(camera_config.saturation));
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Gain' " + to_string(camera_config.gain));
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Sharpness' " + to_string(camera_config.sharpness));
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Exposure (Absolute)' " + to_string(camera_config.exposure));
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Focus, Auto' 0 ");
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Exposure, Auto' 1 ");
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'White Balance Temperature, Auto' 0");
        executeCommand("uvcdynctrl -d video" + to_string(camera) + " -v -s 'Backlight Compensation' 0");
    }
 
}