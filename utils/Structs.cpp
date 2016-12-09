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


}