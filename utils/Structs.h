#ifndef STRUCT_H_
#define STRUCT_H_

#include "../Header.h"
#include "opencv2/highgui/highgui.hpp"
#include <iomanip>
#include <SFML/Graphics.hpp>

namespace rodetas{

typedef cv::Point Point;
typedef cv::Point2f Point2f;
typedef pair<int, int> Pwm;

enum{H, S, V};
enum{MIN, MAX, MID};
enum{H_MAX, H_MIN, S_MAX, S_MIN, V_MAX, V_MIN};

enum{player0, player1, player2, team, opponent, ball};
enum{graphicPlayer0, graphicPlayer1, graphicPlayer2, graphicOpponent1, graphicOpponent2, graphicOpponent3, graphicBall};
enum{fieldLine, midleLine, goal1Line, goal2Line};
enum{titleInformation, fpsInformation, gameStatusInformation, connectionStatusInformation};

struct Command{
	char direcao;
	int pwm1;
	int pwm2;

	Command(){
		direcao = 'P';
		pwm1 = pwm2 = 0;
	}

	Command(const Command &m){
		direcao = m.direcao;
		pwm1 = m.pwm1;
		pwm2 = m.pwm2;
	}	

	Command(char d, int p1, int p2){
		direcao = d;
		pwm1 = p1;
		pwm2 = p2;
	}
};

struct Hsv{

	int h[3];
	int s[3];
	int v[3];
	int variationH_MAX;
	int variationS_MAX;
	int variationV_MAX;
	int variationH_MIN;
	int variationS_MIN;
	int variationV_MIN;

	Hsv(){
		for(int i = 0 ; i < 3 ; i++){
			h[i] = s[i] = v[i] = 0;
		}
	};

	void setH(int pointH){
		h[MIN] = pointH - variationH_MIN;
    	h[MAX] = pointH + variationH_MAX;
    	h[MID] = pointH;
		if (h[MIN] < 0) h[MIN] = 0;
		if (h[MAX] > 255) h[MAX] = 255;
	}

	void setS(int pointS){
		s[MIN] = pointS * (1 - variationS_MIN * 0.01);
    	s[MAX] = pointS * (1 + variationS_MAX * 0.01);
    	s[MID] = pointS;
    	if (s[MIN] < 0) s[MIN] = 0;
		if (s[MAX] > 255) s[MAX] = 255;
	}

	void setV(int pointV){
		v[MIN] = pointV * (1 - variationV_MIN * 0.01);
    	v[MAX] = pointV * (1 + variationV_MAX * 0.01);
    	v[MID] = pointV;
    	if (v[MIN] < 0) v[MIN] = 0;
		if (v[MAX] > 255) v[MAX] = 255;
	}

	void print(){
		cout << "H: " << h[0] << "\t" << h[1] << "\t" << h[2] << endl;
		cout << "S: " << s[0] << "\t" << s[1] << "\t" << s[2] << endl;
		cout << "V: " << v[0] << "\t" << v[1] << "\t" << v[2] << endl;
		cout << "Range H_MAX: " << variationH_MAX << endl;
		cout << "Range S_MAX: " << variationS_MAX << endl;
		cout << "Range V_MAX: " << variationV_MAX << endl;
		cout << "Range H_MIN: " << variationH_MIN << endl;
		cout << "Range S_MIN: " << variationS_MIN << endl;
		cout << "Range V_MIN: " << variationV_MIN << endl << endl;
	};
};

struct Rgb{
	float r;
	float g;
	float b;

	void print(){
		cout << " B: " << b;
		cout << " G: " << g;
		cout << " R: " << r << endl;
	};
};

struct Object{
	
	int id;	
	int x;
	int y;
	int angle;

	Object(){
		x = y = angle = 0;
		id = -1;
	};

	void print(){
		cout << " X: " << x;
		cout << " Y: " << y;
		cout << " ANGLE: " << angle << endl;
	};

	void clear(){
		x = y = angle = 0;
	};

	bool isNull(){
		return (x == 0 && y == 0);
	};
};

struct BlobsContours {
	vector<Point2f>	center;
    vector<float>	radius;
    vector<Point2f> cutPoint1;
    vector<Point2f> cutPoint2;

    void print(int i){
		cout << "CENTER   [" << i << "]: " << center[i] << endl;
		cout << "RADIUS   [" << i << "]: " << radius[i] << endl;
		cout << "CUTPOINT [" << i << "]: " << cutPoint1[i] << cutPoint2[i] << endl << endl;
	};
};

struct Information{
	string strategy;
	char move;
	int pwm1, pwm2, angle, x, y;
	float sin, cos;

	void print(){
		cout << strategy << " - " << move << endl;
		cout << "PWM: " << pwm1 << " | " << pwm2 << endl;
		cout << "Angle: " << angle << endl;
		cout << "Point: {" << x << " , " << y << "}" << endl;
		cout << "Sin: " << sin << endl;
		cout << "Cos: " << cos << endl << endl;
	};

	void update(char m, int p1, int p2, int a, int px, int py, float sinAngle, float cosAngle){
		move = m;
		pwm1 = p1;
		pwm2 = p2;
		angle = a;
		x = px;
		y = py;
		sin = sinAngle;
		cos = cosAngle;
	};

};

struct CameraConfiguration {
	int brightness;
	int contrast;
	int saturation;
	int gain;
	int sharpness;
	int exposure;

	CameraConfiguration(){
		brightness = contrast = saturation = -1;
		gain = sharpness = exposure = -1;
	}
};

template <typename T>
string toString(const T value, const int n){
    std::ostringstream out;
    out << setprecision(n) << value;
	return out.str();
}

template <typename T, typename U>
float distance(const T a, const U b){
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

template <typename T, typename U>
float calcAngle(const T a, const U b){
	return atan2(a.y - b.y, a.x - b.x)*RADIAN_TO_DEGREE;
	//atan2(attackDestination.y - robot.y, attackDestination.x - robot.x)*RADIAN_TO_DEGREE 
}

template <typename T, typename U>
float calcSen(const T a, const U b){
	return sin((calcAngle(b,a) - b.angle)/RADIAN_TO_DEGREE);
}

template <typename T, typename U>
float calcCos(const T a, const U b){
	return cos((calcAngle(b,a) - b.angle)/RADIAN_TO_DEGREE);
}

int getCameraNumber();
string executeCommand(string cmd);
Point changeCordinates(Point, Point, Point);

}

#endif
