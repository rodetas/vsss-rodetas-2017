#include "Manipulation.h"

Manipulation::Manipulation(){
	colorsHSV.resize(6);
	colorsRGB.resize(6);

	loadCalibration();
}

/*
 * Method for saving all parameters calibrated
 */
void Manipulation::saveCalibration(vector<Hsv> &colorsHSV, vector<Rgb> &colorsRGB, Point point_cut_field_1, 
		Point point_cut_field_2, Point goal, int angle_image_rotation, bool cameraOn){

    ofstream output;
    output.open(instance_path);

    if(!output.is_open()){
        cout << "Arquivo nao existente. Criando novo..." << endl;
    } else {

        for(int i=0 ; i<6 ; i++){
            output << colorsHSV[i].h[0] << "\t" << colorsHSV[i].h[1] << "\t" << colorsHSV[i].h[2] << "\t";
            output << colorsHSV[i].s[0] << "\t" << colorsHSV[i].s[1] << "\t" << colorsHSV[i].s[2] << "\t";
            output << colorsHSV[i].v[0] << "\t" << colorsHSV[i].v[1] << "\t" << colorsHSV[i].v[2] << "\t";
            output << colorsRGB[i].b    << "\t" << colorsRGB[i].g    << "\t" << colorsRGB[i].r    << "\t";
            output << colorsHSV[i].variationH_MAX << "\t" << colorsHSV[i].variationS_MAX << "\t" << colorsHSV[i].variationV_MAX << "\t";
            output << colorsHSV[i].variationH_MIN << "\t" << colorsHSV[i].variationS_MIN << "\t" << colorsHSV[i].variationV_MIN << "\t" << endl;
        }

        output << point_cut_field_1.x << "\t" << point_cut_field_1.y << "\t";
        output << point_cut_field_2.x << "\t" << point_cut_field_2.y << "\t";
        output << int(goal.y) << "\t" << int(goal.x) << "\t";
        output << angle_image_rotation << "\t" << cameraOn;
        
        cout << "Calibration Saved!" << endl;
    }

    output.close();
}

void Manipulation::saveCoordinates(vector<rodetas::Object>& positions){

    ofstream arquivo(filename, std::ofstream::app);

    for(int i=0 ; i<positions.size() ; i++){
        arquivo << positions[i].x << " " << positions[i].y << " " << positions[i].angle << endl;
    }

    arquivo << endl;

    arquivo.close();
}

void Manipulation::saveCameraConfig(CameraConfiguration cam_config){

    ofstream arquivo("../files/instances/cameraConfig.txt", std::ofstream::trunc);
    if (!arquivo.is_open()){
        cout << "Camera Config SAVE - file not found" << endl;
    }

    arquivo << cam_config.brightness;
    arquivo << " ";
    arquivo << cam_config.contrast;
    arquivo << " ";
    arquivo << cam_config.saturation;
    arquivo << " ";
    arquivo << cam_config.gain;
    arquivo << " ";
    arquivo << cam_config.sharpness;
    arquivo << " ";
    arquivo << cam_config.exposure;

    arquivo.close();
}

CameraConfiguration Manipulation::loadCameraConfig(){

    CameraConfiguration cam;

    ifstream arquivo("../files/instances/cameraConfig.txt", std::ifstream::in);
    if (!arquivo.is_open()){
        cout << "Camera Config LOAD - file not found" << endl;
    }
    arquivo >> cam.brightness;
    arquivo >> cam.contrast;
    arquivo >> cam.saturation;
    arquivo >> cam.gain;
    arquivo >> cam.sharpness;
    arquivo >> cam.exposure;

    arquivo.close();

    return cam;
}

/*
 * Method for loading all parameters calibrated
 */
pair<vector<Hsv>, vector<Rgb>> Manipulation::loadCalibration(){
    string num;
    int i = 0, j = 0;
    int numeros[12];
    ifstream input;

    input.open(instance_path);

    if(!input.is_open()){
        cout << "Arquivo nao existente. Utilizando padrao..." << endl;
        input.close();
        input.open(standart_instance);
    }

    for(int i=0 ; i<6 ; i++){
        input >> colorsHSV[i].h[0];
        input >> colorsHSV[i].h[1];
        input >> colorsHSV[i].h[2];
        input >> colorsHSV[i].s[0];
        input >> colorsHSV[i].s[1];
        input >> colorsHSV[i].s[2];
        input >> colorsHSV[i].v[0];
        input >> colorsHSV[i].v[1];
        input >> colorsHSV[i].v[2];
        input >> colorsRGB[i].b;
        input >> colorsRGB[i].g;
        input >> colorsRGB[i].r;
        input >> colorsHSV[i].variationH_MAX;
        input >> colorsHSV[i].variationS_MAX;
        input >> colorsHSV[i].variationV_MAX;
        input >> colorsHSV[i].variationH_MIN;
        input >> colorsHSV[i].variationS_MIN;
        input >> colorsHSV[i].variationV_MIN;
    }

    input >> point_cut_field_1.x;
    input >> point_cut_field_1.y;
    input >> point_cut_field_2.x;
    input >> point_cut_field_2.y;
    input >> goal.y;
    input >> goal.x;
    input >> angle_image_rotation;
    input >> cameraOn;

    input.close();

    return make_pair(colorsHSV, colorsRGB);
}

void Manipulation::printCalibration(){
    for(int i=0 ; i<colorsHSV.size() ; i++){
        colorsHSV[i].print();
    }

    for(int i=0 ; i<colorsRGB.size() ; i++){
        colorsRGB[i].print();
    }

    cout << point_cut_field_1 << endl;
    cout << point_cut_field_2 << endl;

}

Point Manipulation::getImageSize(){
    Point size;
    size.x = abs(point_cut_field_2.x - point_cut_field_1.x);
    size.y = abs(point_cut_field_2.y - point_cut_field_1.y);
    return size;
}

vector<Hsv> Manipulation::getColorsHsv(){
	return colorsHSV;
}

vector<Rgb> Manipulation::getColorsRgb(){
	return colorsRGB;
}

vector<Rgb> Manipulation::getColorsRgbCairo(){

    for(int i=0 ; i<colorsRGB.size() ; i++){
        colorsRGB[i].r /= 255;
        colorsRGB[i].g /= 255;
        colorsRGB[i].b /= 255;
    }

    return colorsRGB;
}

Point Manipulation::getPointField1(){
	return point_cut_field_1;
}

Point Manipulation::getPointField2(){
	return point_cut_field_2;
}

int Manipulation::getAngleImage(){
	return angle_image_rotation;
}

bool Manipulation::getCameraOn(){
	return cameraOn;
}

Point Manipulation::getGoal(){
    return goal;
}

void Manipulation::setFileName(string name){    

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y_%I:%M",timeinfo);
    string str(buffer);

    this->filename = "../../files/instances/" + name + "-" + str + ".txt";
}