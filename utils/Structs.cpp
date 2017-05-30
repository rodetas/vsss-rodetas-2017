#include "Structs.h"

namespace rodetas{

    int getCameraNumberScript(){
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

    void updateCameraValuesScript(CameraConfiguration camera_config, int camera){
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl brightness=" + to_string(camera_config.brightness));
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl contrast=" + to_string(camera_config.contrast));
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl saturation=" + to_string(camera_config.saturation));
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl gain=" + to_string(camera_config.gain));
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl sharpness=" + to_string(camera_config.sharpness));
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl exposure_absolute=" + to_string(camera_config.exposure));
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl focus_auto=0 ");
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl exposure_auto=1 ");
        executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + " --set-ctrl backlight_compensation=0");
    }

    void defaultCameraScript(int camera){
        cout << "Default Camera ..." << endl;        
        string all_comands = executeCommand("v4l2-ctl -list | awk '{print $1 $7}' | grep default | sed 's/default//g'");
        
        for (int i = 0; i < all_comands.size(); i++){
            string comand = "";
            while (all_comands[i] != '\n'){
                comand = comand + all_comands[i];
                i++;
            }
            executeCommand("v4l2-ctl -d /dev/video" + to_string(camera) + "--set-ctrl " + comand);
        }
    }
 
}