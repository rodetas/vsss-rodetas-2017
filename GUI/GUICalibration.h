#ifndef GUICALIBRATION_H_
#define GUICALIBRATION_H_

#include "../Header.h"
#include "../utils/Structs.h"
#include "opencv2/imgproc/imgproc.hpp"

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

using namespace rod;

class Calibration;

class GUICalibration {

private:

    Calibration *calibration;

    sf::RenderWindow window;
    sf::RectangleShape gridView1;
    sf::RectangleShape gridView2;
    sf::RectangleShape gridView3;
    sf::RectangleShape gridView4;
    sf::RectangleShape cutField;
    sf::Font fontTitle;
    sf::Text rodetasTitle;
    sf::Text rotationText;
    sf::Text rangeHText;
    sf::Text rangeSText;
    sf::Text rangeVText;
    sf::Image sfmlRGBImage;
    sf::Texture sfmlRGBTexture;
    sf::ConvexShape fieldConvexShape;
    sf::Vector2i pixelCut1;
    sf::Vector2i pixelCut2;
    sf::View standardView;
    sf::View zoomView;
    sf::VertexArray blobPoints;

    tgui::Gui gui;
    tgui::Slider::Ptr sliderRotation;
    tgui::Slider::Ptr sliderH_MAX;
    tgui::Slider::Ptr sliderS_MAX;
    tgui::Slider::Ptr sliderV_MAX;
    tgui::Slider::Ptr sliderH_MIN;
    tgui::Slider::Ptr sliderS_MIN;
    tgui::Slider::Ptr sliderV_MIN;
    tgui::Tab::Ptr tab;
    tgui::RadioButton::Ptr radioButton1;
    tgui::RadioButton::Ptr radioButton2;
    tgui::Theme::Ptr theme;

    Point screen;
    Point halfScreen;
    bool cutOn;
    bool changeSlider; //criada por causa do tgui que considera setar valor do botão como uma ação
    bool changeRadioButton; //criada por causa do tgui que considera setar valor do botão como uma ação
    double screenPercentage;
    double rate;
    vector<int> sliderMax;


public:
    GUICalibration();
    void createWindow();
    void reopenWindow();
    void closeWindow();
    void imageOpencvToSfml(cv::Mat, cv::Mat);
    void events();
    void interfaceCreateTGUI();
    void interfaceCreateSFML();
    void fieldCreate();
    void GUI();

    void setSlider(int, int, int, int, int, int);
    void setGUI(Calibration*);
};

#include "../Calibration.h"
#endif