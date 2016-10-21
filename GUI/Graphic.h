#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include "../utils/OpenGL.h"

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include "widgets/Button.h"
#include "../CRUD/Manipulation.h"

using namespace rod;

class Graphic : public OpenGL{
private:

    Manipulation manipulation;

    sf::RenderWindow window;
    sf::RenderWindow windowLogConsole;
    sf::ConvexShape fieldConvexShape;

    vector<Button> buttons;

    vector<sf::RectangleShape> field;
    vector<sf::Text> textScreen;
    vector<sf::CircleShape> targets;

    vector<RobotOpenGL> robot;
    vector<Information> information;

    OpenGL openGLWindowLogConsole;
    Point2f windowSizeLogConsole;

    sf::Texture background;
    sf::Texture textureBall;

    sf::CircleShape drawBall;

    sf::RectangleShape top;
    sf::RectangleShape under;
    sf::RectangleShape left;
    sf::RectangleShape right;

    sf::Font fontTitle;
    sf::Font fontText;

    sf::Clock clockVector;
    sf::Clock clockSave;

    Point2f imageSize;
    Point2f fieldSize;
    Point2f screenSize;
    Point2f viewSize; 
    Point2f origin;
    Point2f halfScreen;
    Point2f goal;

    string testMovement;

    int topMargin;
    int fpsCount;

    float screenPercentage;
    float rateFieldSize;
    float sizeRobot;
    float radiusBall;
    float power;
    float powerCurve;

    vector<Hsv> colorsHSV;
    vector<Rgb> colorsRGB;
    vector<Object> objects;
    vector<Object> lastObjects;
    
    tgui::Gui gui;

    bool pause;
    bool menu;
    bool penalti;
    bool close;
    bool connectionStatus;
    bool logConsoleOpen;
    bool savingPositions;
    bool rotateField;


public:
    Graphic();
    void interfaceCreateSFML();
    void interfaceCreateTGUI();
    void interfaceCreateButton();
    void textsCreate();
    void fieldCreate();
    void events();
    void windowDraw();
    void cornerTest();
    void loadTextures();
    void gameInformation();
    void initialize();
    void updateInformations();
    bool isPause();
    bool isPenalti();
    void logConsole();
    void updateLogConsole();

	template <typename typePoint>
	typePoint changeCoordinate(typePoint);

    string getTestMovement();
    float getPowerCurve();
    float getPower();
    bool getMenu();
    bool getClose();
    bool  getRotateField();

    void saveCoordinates();
    void setMenu(bool);
    void setObjects(vector<Object>);
    void setFps(int);
    void setTargets(vector<Point>);
    void setInformation(vector<Information>);
    void setConnectionStatus(bool);
};
#endif
