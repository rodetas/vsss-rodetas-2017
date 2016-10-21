#ifndef MANIPULATION
#define MANIPULATION

#include "../Header.h"
#include "../utils/Structs.h"
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace rod;

class Manipulation{

private:
	//variaveis de calibragem
	vector<Hsv> colorsHSV;
	vector<Rgb> colorsRGB;
	vector<float> blobSize;
	Point pointCutField1;
	Point pointCutField2;
	Point goal;
	int angleImageRotation;
	bool cameraOn;

	//variaveis de salvar posicoes
	string filename;
	bool created;

public:
	Manipulation();
	void saveCalibration(vector<Hsv>&, vector<Rgb>&, vector<float>&, Point, Point, Point, int, bool);
	void saveCoordinates(vector<Object>&);
	pair<vector<Hsv>, vector<Rgb>> loadCalibration();

	void showCalibration();

	vector<Hsv> getColorsHsv();
	vector<Rgb> getColorsRgb();
	vector<float> getBlobSize();
	Point getPointField1();
	Point getPointField2();
	int getAngleImage();
	bool getCameraOn();
	Point getGoal();
	Point getImageSize();

	void setFileName(string);

};

#endif