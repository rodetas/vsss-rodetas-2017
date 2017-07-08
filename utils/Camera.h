#ifndef CAMERA_H_
#define CAMERA_H_

#include "../Header.h"
#include "Commons.h"


class Camera {

private:
    int camera_number;
    vector<string> camera_comand;
    
    int getNumberCameraScript();
    vector<string> getComandCameraScript();

public:
	Camera();
    void defaultCameraScript();
    void updateCameraValuesScript(rodetas::CameraConfiguration camera_config);
    int getNumber();

};
#endif