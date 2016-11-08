#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include "../utils/Fps.h"

#include <SFML/Graphics.hpp>

using namespace rod;

class Simulator{
	
private:
	Fps fps;
	
	sf::RenderWindow window;
	sf::ConvexShape fieldConvexShape;
	sf::CircleShape ball;
	sf::Texture background;
	sf::Texture textureBall;
	sf::Font fontTitle;
	sf::Font fontText;

	sf::Vector2i positionMouse;
	sf::Vector2f screenSize;
	sf::Vector2f halfScreen;
	sf::Vector2f imageSizeZoom;
	sf::Vector2f goal;
	sf::Vector2f origin;

	vector<sf::Texture> texturesRobots;
	vector<sf::CircleShape> targets;
	vector<sf::Text> textRobots;
	vector<sf::Text> textScreen;
	vector<sf::RectangleShape> field;
	vector<sf::RectangleShape> robots;
	vector<rod::Object> objects;
    vector<string> information;

	float rateZoom;
	float sizeRobot;
	float radiusBall;
	bool simulatorOpened;
	bool moveRobot;
	bool moveBall;
	bool endSimulator;
	bool ballCreated;
	int atualRobot;

public:
	Simulator();
	void initialize();
	void simulate();
	void openWindow();
	void loadTextures();
	bool onBall(sf::CircleShape, int, int);
	bool onField(int, int);
	void robotCreate(int, int);
	void ballCreate(int, int);	
	void textsCreate();
	void fieldCreate();
	void events();
	void drawWindow();

	void setEndSimulator(bool);
    void setInformation(string*);
    void setTargets(vector<Point>);
	bool getEndSimulator();
	vector<rod::Object> getPositions();
};
#endif