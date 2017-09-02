#ifndef STRUCT_H_
#define STRUCT_H_

#include "../Header.h"
#include "opencv2/highgui/highgui.hpp"
#include <iomanip>
#include <stdio.h>
#include <iterator>

namespace rodetas{

typedef cv::Point Point;
typedef cv::Point2i Point2i;

enum{H, S, V};
enum{MIN, MAX, MID};
enum{H_MAX, H_MIN, S_MAX, S_MIN, V_MAX, V_MIN};
enum{PLAYER0, PLAYER1, PLAYER2, TEAM, OPPONENT, BALL};
enum{GRAPHICPLAYER0, GRAPHICPLAYER1, GRAPHICPLAYER2, GRAPHICOPPONENT1, GRAPHICOPPONENT2, GRAPHICOPPONENT3, GRAPHICBALL};

enum{fieldLine, midleLine, goal1Line, goal2Line};
enum{titleInformation, fpsInformation, gameStatusInformation, connectionStatusInformation};

static Point imageSize = Point();
static Point goalSize = Point();
static Point goalArea = Point();

struct PointCut {
	Point2i first;
	Point2i second;

	PointCut (){}

	// initialize making a square area to cut
	PointCut (Point2i point, int side_size){
		first  = cv::Point( point.x - side_size , point.y - side_size );
        second = cv::Point( point.x + side_size , point.y + side_size );
	}
};

struct Command{
	char direcao;
	int pwm1;
	int pwm2;

	Command(){
		direcao = FORWARD_MOVE;
		pwm1 = pwm2 = 0;
	}

	Command(const Command &m){
		direcao = m.direcao;
		pwm1 = m.pwm1;
		pwm2 = m.pwm2;
	}

	Command(int p1, int p2, char d){
		direcao = d;
		pwm1 = p1;
		pwm2 = p2;
	}

	Command& operator=(const Command& c) {
        if (this != &c){
			direcao = c.direcao;
			pwm1 = c.pwm1;
			pwm2 = c.pwm2;
        }

        return *this;
    }

	string to_string(){
		stringstream ss;
		ss << direcao;
		ss << setfill('0') << setw(3) << pwm1;
		ss << setfill('0') << setw(3) << pwm2;
		return ss.str();
	}

	friend std::ostream& operator<< (std::ostream& stream, const Command& c) {
		stream << c.direcao << " " << c.pwm1 << " " << c.pwm2;
		return stream;
    }

	vector<int> to_hex(){
		string cmd = this->to_string();
		vector<int> vec;

		for(int i=0 ; i<cmd.size() ; i++){
			vec.push_back(int(cmd[i]));
		}

		return vec;
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

	void defaultValue(){
		variationH_MAX = 50;
		variationS_MAX = 50;
		variationV_MAX = 50;
		variationH_MIN = 50;
		variationS_MIN = 50;
		variationV_MIN = 50;
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

	Object(int _x, int _y){
		x = _x;
		y = _y;
	}

	Object(Point2i p){
		x = p.x;
		y = p.y;
	}

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

	friend std::ostream& operator<< (std::ostream& stream, const Object& o) {
		stream << "ID: " << o.id << " X:" << o.x << " Y:" << o.y << " ANGLE:" << o.angle;
		return stream;
    }
};

struct Position {
    vector<float>	 radius;
	vector<Point2i>	 center;
	
	int size(){
		return center.size();
	};

	void changeCoordinateToGlobal(int i, PointCut point){
		center[i].x += point.first.x;
        center[i].y += point.first.y;
	};

    void print(int i){
		cout << "CENTER   [" << i << "]: " << center[i] << endl;
		cout << "RADIUS   [" << i << "]: " << radius[i] << endl;
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

	void print(){
		cout << "brightness = " << brightness << endl;
		cout << "contrast = " << contrast << endl;
		cout << "saturation = " << saturation << endl;
		cout << "gain = " << gain << endl;
		cout << "sharpness = " << sharpness << endl;
		cout << "exposure = " << exposure << endl;
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

template <typename T, typename U, typename X>
bool insideCircle(const T a, const U b, const X r){
	return distance(a,b) < (r);
}

template <typename T, typename U>
float calcAngle(const T a, const U b){
	return atan2(a.y - b.y, a.x - b.x)*RADIAN_TO_DEGREE;
}

template <typename T, typename U>
float calcSen(const T a, const U b){
	return sin((calcAngle(b,a) - b.angle)/RADIAN_TO_DEGREE);
}

template <typename T, typename U>
float calcCos(const T a, const U b){
	return cos((calcAngle(b,a) - b.angle)/RADIAN_TO_DEGREE);
}

string executeCommand(string cmd);
Point changeCordinates(Point, Point, Point);
}

#endif