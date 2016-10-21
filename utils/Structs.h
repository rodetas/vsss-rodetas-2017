#ifndef STRUCT_H_
#define STRUCT_H_

#include "../Header.h"
#include "opencv2/highgui/highgui.hpp"
#include <iomanip>
#include <SFML/Graphics.hpp>

namespace rod{

typedef cv::Point Point;
typedef cv::Point2f Point2f;
typedef pair<string, int[2]> Command;
typedef pair<int, int> Pwm;
typedef sf::Clock Clock;

enum{H, S, V};
enum{H_MAX, H_MIN, S_MAX, S_MIN, V_MAX, V_MIN};

enum{player0, player1, player2, team, opponent, ball};
enum{graphicPlayer0, graphicPlayer1, graphicPlayer2, graphicOpponent1, graphicOpponent2, graphicOpponent3, graphicBall};
enum{fieldLine, midleLine, goal1Line, goal2Line};
enum{titleInformation, fpsInformation, gameStatusInformation, connectionStatusInformation};

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
		h[0] = pointH - variationH_MIN;
    	h[1] = pointH + variationH_MAX;
    	h[2] = pointH;
		if (h[0] < 0) h[0] = 0;
		if (h[0] > 255) h[0] = 255;
	}

	void setS(int pointS){
		s[0] = pointS * (1 - variationS_MIN * 0.01);
    	s[1] = pointS * (1 + variationS_MAX * 0.01);
    	s[2] = pointS;
    	if (s[0] < 0) s[0] = 0;
		if (s[0] > 255) s[0] = 255;

	}

	void setV(int pointV){
		v[0] = pointV * (1 - variationV_MIN * 0.01);
    	v[1] = pointV * (1 + variationV_MAX * 0.01);
    	v[2] = pointV;
    	if (v[0] < 0) v[0] = 0;
		if (v[0] > 255) v[0] = 255;

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
	int r;
	int g;
	int b;

	void print(){
		cout << "B: " << b;
		cout << " G: " << g;
		cout << " R: " << r << endl;
	};
};

struct Object{
	float x;
	float y;
	float angle;

	int id;

	void print(){
		cout << "X: " << x;
		cout << " Y: " << y;
		cout << " ANGLE: " << angle << endl;
	};

	void clear(){
		x = y = angle = 0;
	};

	bool isNull(){
		return (x == 0 && y == 0);
	}

	Object(){
		x = y = angle = 0;
		id = -1;
	};
};

struct BlobsContours {
	vector<Point2f>		center;
    vector<float>		radius;
    vector<Point2f>   cutPoint1;
    vector<Point2f>   cutPoint2;

    void print(int i){
		cout << "CENTER   [" << i << "]: " << center[i] << endl;
		cout << "RADIUS   [" << i << "]: " << radius[i] << endl;
		cout << "CUTPOINT [" << i << "]: " << cutPoint1[i] << cutPoint2[i] << endl << endl;
	};
};

struct RobotOpenGL{
	float x;
	float y;
	float angle;
	Rgb colorTeam;
	Rgb colorPlayer;

	RobotOpenGL(){
		x = y = angle = 0;
	};
};

struct Information{
	string strategy, move;
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

	void update(string m, int p1, int p2, int a, int px, int py, float sinAngle, float cosAngle){
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
void configureCamera();
string executeCommand(string cmd);

}

#endif
