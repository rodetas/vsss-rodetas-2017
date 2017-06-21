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
	Point point_cut_field_1;
	Point point_cut_field_2;
	Point goal;
	int angle_image_rotation;
	bool cameraOn;

	string filename;

	const string instance_path = "files/instances/fileCalibration.txt";
	const string standart_instance = "files/instances/standartInstance.txt";

public:

	/**
	 * Default Constructor - automatically loads the instance file
	 */
	Manipulation();

	/**
	 * Saves the calibration
	 * \param hsv: receives a hsv vector containing the robot colors references
	 * \param rgb: receives a rgb vector containing the robot colors references
	 * \param p1: receives the point cut field 1
	 * \param p2: receibes the point cut field 2
	 * \param p3: receives the goal point
	 * \param angle: receives the rotation angle
	 * \param cam: receives true if camera is on
	 */
	void saveCalibration(vector<Hsv>& hsv, vector<Rgb>& rgb, Point p1, Point p2, Point p3, int angle, bool cam);

	/**
	 * Saves the camera parameters
	 * \param cam_config: receives the struct containing the data
	 */
	void saveCameraConfig(CameraConfiguration cam_config);

	/**
	 * Saves the coordinates captured during the gama
	 * \param objects: receive the vector containing all the positions captured during the game
	 */
	void saveCoordinates(vector<rodetas::Object>&);

	/**
	 * Loads the calibration file
	 * \return a pair containing a hsv vector and rgb vector
	 */
	pair<vector<Hsv>, vector<Rgb>> loadCalibration();

	/**
	 * Loads the camera configuration file
	 * \return the struct CameraConfiguration which contains all the informations
	 */
	CameraConfiguration loadCameraConfig();

	/**
	 * Print on the screen the calibration loaded so far
	 */
	void printCalibration();

	/**
	 * \return The vector containing the HSV colors
	 */
	vector<Hsv> getColorsHsv();

	/**
	 * \return The vector containing the RGB colors
	 */
	vector<Rgb> getColorsRgb();

	/**
	 *
	 */
	vector<Rgb> getColorsRgbCairo();

	/**
	 * \return The first point which delimitates the board	
	 */
	Point getPointField1();

	/**
	 * \return The second point which delimitates the board
	 */
	Point getPointField2();

	/**
	 * \return The image angle rotation
	 */
	int getAngleImage();

	/**
	 * \return true if should use the camera
	 */
	bool getCameraOn();

	/**
	 * \return The point which delimitates the goal
	 */
	Point getGoal();

	/**
	 * \return The point containing the board size
	 */
	Point getImageSize();

	/**
	 * Sets the file name for saving all the game coordinates
	 */
	void setFileName(string);
};

#endif