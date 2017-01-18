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


}