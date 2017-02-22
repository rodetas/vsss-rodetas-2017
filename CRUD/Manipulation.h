#ifndef MANIPULATION
#define MANIPULATION

#include "../Header.h"
#include "../utils/Structs.h"
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace rodetas;

class Manipulation{

private:
	//variaveis de calibragem
	vector<Hsv> colorsHSV;
	vector<Rgb> colorsRGB;
	vector<float> blobSize;
	Point point_cut_field_1;
	Point point_cut_field_2;
	Point goal;
	int angle_image_rotation;
	bool cameraOn;

	//variaveis de salvar posicoes
	string filename;
	bool created;

public:
	Manipulation();
	void saveCalibration(vector<Hsv>&, vector<Rgb>&, Point, Point, Point, int, bool);
	void saveCameraConfig(CameraConfiguration cam_config);
	void saveCoordinates(vector<rodetas::Object>&);

	pair<vector<Hsv>, vector<Rgb>> loadCalibration();
	CameraConfiguration loadCameraConfig();

	void showCalibration();

	vector<Hsv> getColorsHsv();
	vector<Rgb> getColorsRgb();
	vector<Rgb> getColorsRgbCairo();
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