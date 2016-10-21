#include "Manipulation.h"

Manipulation::Manipulation(){
	colorsHSV.resize(6);
	colorsRGB.resize(6);
	blobSize.resize(6);

    created = false;

	loadCalibration();
}

/*
 * Method for saving all parameters calibrated
 */
void Manipulation::saveCalibration(vector<Hsv> &colorsHSV, vector<Rgb> &colorsRGB, vector<float> &blobSize, Point pointCutField1, 
		Point pointCutField2, Point goal, int angleImageRotation, bool cameraOn){

    ofstream output;
    output.open(instancePath);

    if(!output.is_open()){
        cout << "FALHA ARQUIVO" << endl;
    } else {

        for(int i=0 ; i<6 ; i++){
            output << colorsHSV[i].h[0] << "\t" << colorsHSV[i].h[1] << "\t" << colorsHSV[i].h[2] << "\t";
            output << colorsHSV[i].s[0] << "\t" << colorsHSV[i].s[1] << "\t" << colorsHSV[i].s[2] << "\t";
            output << colorsHSV[i].v[0] << "\t" << colorsHSV[i].v[1] << "\t" << colorsHSV[i].v[2] << "\t";
            output << colorsRGB[i].b    << "\t" << colorsRGB[i].g    << "\t" << colorsRGB[i].r    << "\t";
            output << colorsHSV[i].variationH_MAX << "\t" << colorsHSV[i].variationS_MAX << "\t" << colorsHSV[i].variationV_MAX << "\t";
            output << colorsHSV[i].variationH_MIN << "\t" << colorsHSV[i].variationS_MIN << "\t" << colorsHSV[i].variationV_MIN << "\t";
            output << blobSize[i] << endl;
        }

        output << pointCutField1.x << "\t" << pointCutField1.y << "\t";
        output << pointCutField2.x << "\t" << pointCutField2.y << "\t";
        output << int(goal.y) << "\t" << int(goal.x) << "\t";
        output << angleImageRotation << "\t" << cameraOn;
        
        cout << "Calibration Saved!" << endl;
    }

    output.close();
}

void Manipulation::saveCoordinates(vector<Object>& positions){

    ofstream arquivo(filename, std::ofstream::app);

    for(int i=0 ; i<positions.size() ; i++){
        arquivo << positions[i].x << " " << positions[i].y << " " << positions[i].angle << endl;
    }

    arquivo << endl;

    arquivo.close();
}

/*
 * Method for loading all parameters calibrated
 */
pair<vector<Hsv>, vector<Rgb>> Manipulation::loadCalibration(){
    string num;
    int i = 0, j = 0;
    int numeros[12];
    ifstream input;

    input.open(instancePath);

    if(!input.is_open()){
        cout << "FALHA ARQUIVO" << endl;
    } else {

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
            input >> blobSize[i];
        }

        input >> pointCutField1.x;
        input >> pointCutField1.y;
        input >> pointCutField2.x;
        input >> pointCutField2.y;
        input >> goal.y;
        input >> goal.x;
        input >> angleImageRotation;
        input >> cameraOn;

    }

    input.close();

    return make_pair(colorsHSV, colorsRGB);
}

void Manipulation::showCalibration(){
    for(int i=0 ; i<colorsHSV.size() ; i++){
        colorsHSV[i].print();
    }

    for(int i=0 ; i<colorsRGB.size() ; i++){
        colorsRGB[i].print();
    }

    cout << pointCutField1 << endl;
    cout << pointCutField2 << endl;

}

Point Manipulation::getImageSize(){
    Point size;
    size.x = abs(pointCutField2.x - pointCutField1.x);
    size.y = abs(pointCutField2.y - pointCutField1.y);
    return size;
}

vector<Hsv> Manipulation::getColorsHsv(){
	return colorsHSV;
}

vector<Rgb> Manipulation::getColorsRgb(){
	return colorsRGB;
}

vector<float> Manipulation::getBlobSize(){
	return blobSize;
}

Point Manipulation::getPointField1(){
	return pointCutField1;
}

Point Manipulation::getPointField2(){
	return pointCutField2;
}

int Manipulation::getAngleImage(){
	return angleImageRotation;
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

    this->filename = "files/instances/" + name + "-" + str + ".txt";
}