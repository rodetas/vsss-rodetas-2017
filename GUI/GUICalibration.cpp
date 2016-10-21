#include "GUICalibration.h"

/*
 * class constructor
 */
GUICalibration::GUICalibration(){ 
    fontTitle.loadFromFile(fontTitlePath);
    theme = std::make_shared<tgui::Theme>(tguiThemePath);

    changeSlider = true;
    changeRadioButton = true;
    screenPercentage = 0.6;

    sliderMax.resize(3);
    sliderMax[H] = 60;
    sliderMax[S] = 100;
    sliderMax[V] = 100;
}

/*
 * draw GUI and receive events 
 */
void GUICalibration::GUI(){
    
    window.clear(sf::Color(0, 0, 0));

    events();
    
    window.draw(fieldConvexShape);
    window.draw(cutField);   
    window.draw(rodetasTitle);
    window.draw(rotationText);
    //window.draw(rangeSText);
    //window.draw(rangeVText);
    //window.draw(rangeHText);
    window.draw(gridView1);
    window.draw(gridView2);
    window.draw(gridView3);
    window.draw(gridView4);
    window.draw(blobPoints);

    gui.draw();

    window.display();
}

/*
 * Method for create window and initialize the values of elements position
 */
void GUICalibration::createWindow(){
    screen.x = sf::VideoMode::getDesktopMode().width;
    screen.y = sf::VideoMode::getDesktopMode().height;
    halfScreen.x = (screen.x)/2;

    window.create(sf::VideoMode(screen.x, screen.y, sf::VideoMode::getDesktopMode().bitsPerPixel), "Rodetas", sf::Style::Close);
    gui.setWindow(window);

    calibration->margin.y = 100;
    calibration->imageSizeZoom.y = screen.y - calibration->margin.y - 200; //200 - widgets positions
    calibration->imageSizeZoom.x = calibration->imageSizeZoom.y * 1.6;
    calibration->margin.x = halfScreen.x - calibration->imageSizeZoom.x/2; 
    calibration->updateTab();
    calibration->changedColor = false;

    interfaceCreateTGUI();
    interfaceCreateSFML();
    fieldCreate();
}

/*
 * Method to reopen closed window
 */
void GUICalibration::reopenWindow(){
    calibration->changedColor = false;
    window.create(sf::VideoMode(screen.x, screen.y, sf::VideoMode::getDesktopMode().bitsPerPixel), "Rodetas", sf::Style::Close);
}

/*
 * Method to close window
 */
void GUICalibration::closeWindow(){
    window.close();
}

/*
 * Method to convert opencv image to sfml image 
 */
void GUICalibration::imageOpencvToSfml(cv::Mat opencvImageRGB, cv::Mat opencvHandledImageRGB){
    sfmlRGBImage.create(opencvImageRGB.cols, opencvImageRGB.rows, opencvImageRGB.ptr());
    sfmlRGBTexture.loadFromImage(sfmlRGBImage);
    fieldConvexShape.setTexture(&sfmlRGBTexture);

    blobPoints = calibration->drawBlobs(opencvHandledImageRGB);
}

/*
 * Method for events SFML
 *      - Key pressed   - Mouse button pressed  - Mouse moved
 */
void GUICalibration::events(){
    sf::Event event;
    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed){
            calibration->endCalibration = true;
            window.close();
        }

        // left button
        else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            sf::Vector2i pixelPointSFML = sf::Mouse::getPosition(window);
            sf::IntRect insideField(calibration->margin.x, calibration->margin.y, calibration->imageSizeZoom.x, calibration->imageSizeZoom.y);
            if(insideField.contains(pixelPointSFML)){
                setSlider(10,10,10,10,10,10);
                calibration->changedColor = true;
                calibration->updateColorPixel(Point(pixelPointSFML.x, pixelPointSFML.y));
                calibration->updateRGB();
            }
        }

        // right button
        else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
            pixelCut1 = sf::Mouse::getPosition(window);
            sf::IntRect insideField(calibration->margin.x, calibration->margin.y, calibration->imageSizeZoom.x, calibration->imageSizeZoom.y);
            
            if(insideField.contains(pixelCut1)){
                cutField.setPosition(sf::Vector2f(pixelCut1.x,pixelCut1.y));
                cutField.setSize(sf::Vector2f(0,0));
                cutOn = true;
            } else {
                cutOn = false;
            }
        }

        // right button moved
        else if(event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Right)){
            if(cutOn){
                pixelCut2 = sf::Mouse::getPosition(window);            
                
                if (pixelCut2.x-calibration->margin.x > calibration->imageSizeZoom.x) pixelCut2.x = calibration->imageSizeZoom.x+calibration->margin.x;
                if (pixelCut2.y-calibration->margin.y > calibration->imageSizeZoom.y) pixelCut2.y = calibration->imageSizeZoom.y+calibration->margin.y;
                if (pixelCut2.x-calibration->margin.x < 0) pixelCut2.x = calibration->margin.x;
                if (pixelCut2.y-calibration->margin.y < 0) pixelCut2.y = calibration->margin.y;

                cutField.setSize(sf::Vector2f(pixelCut2.x-pixelCut1.x, pixelCut2.y-pixelCut1.y));
            }
        }

        // space - save points to cut
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){

            calibration->pointCutField1 = calibration->changeCordinates( Point(pixelCut1.x - calibration->margin.x, pixelCut1.y - calibration->margin.y) );
            calibration->pointCutField2 = calibration->changeCordinates( Point(pixelCut2.x - calibration->margin.x, pixelCut2.y - calibration->margin.y) );
        
            cutField.setSize(sf::Vector2f(0,0));
        }
        
        // g - save goal coordinates
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G){

            calibration->goal = Point( abs(pixelCut2.x - pixelCut1.x) , abs(pixelCut2.y - pixelCut1.y));

            cutField.setSize(sf::Vector2f(0,0));

            fieldCreate();
        }
        
        // enter - save calibration
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return){
            calibration->endCalibration = true;
            window.close();
        }

        // esc - exit program
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
            window.close();
            exit(0);
        }

        // r resize cut
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R){
            calibration->pointCutField1 = {0,0};
            calibration->pointCutField2 = calibration->imageSize;
        } 
        
        // right - move tab to the right 
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
        } 
        // left - move tab to the left
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
        }

        gui.handleEvent(event);
    }
}

/*
 * Method for interface create using TGUI library
 */
void GUICalibration::interfaceCreateTGUI(){

    sliderRotation = theme->load("Slider");
    gui.add(sliderRotation, "sliderRotation");
    sliderRotation->setPosition(int(calibration->margin.x + (calibration->imageSizeZoom.x/2) - (calibration->imageSizeZoom.x*3/8)), screen.y - 100 - 90);
    sliderRotation->setSize(int(calibration->imageSizeZoom.x*3/4), 12);
    sliderRotation->setMaximum(360);
    sliderRotation->setMinimum(0);
    sliderRotation->setValue(180);
    sliderRotation->connect("ValueChanged", [&](){ 
        calibration->angleImageRotation = sliderRotation->getValue();
    });  
     
    sliderH_MAX = theme->load("Slider");
    gui.add(sliderH_MAX, "sliderH_MAX");
    sliderH_MAX->setPosition(int(halfScreen.x - calibration->imageSizeZoom.x/8 - calibration->imageSizeZoom.x/3), screen.y - 100 - 45);
    sliderH_MAX->setSize(calibration->imageSizeZoom.x/4, 12);
    sliderH_MAX->setMaximum(sliderMax[H]);
    sliderH_MAX->setMinimum(0);
    sliderH_MAX->setValue(calibration->range[H_MAX]);
    sliderH_MAX->connect("ValueChanged", [&](){
        if (changeSlider){
            calibration->changedColor = true;
            calibration->range[H_MAX] = sliderH_MAX->getValue();
            calibration->updateSlider();
        }
    });    

    sliderS_MAX = theme->load("Slider");
    gui.add(sliderS_MAX, "sliderS_MAX");
    sliderS_MAX->setPosition(int(halfScreen.x - calibration->imageSizeZoom.x/8), screen.y - 100 - 45);
    sliderS_MAX->setSize(calibration->imageSizeZoom.x/4, 12);
    sliderS_MAX->setMaximum(sliderMax[S]);
    sliderS_MAX->setMinimum(0);
    sliderS_MAX->setValue(calibration->range[S_MAX]);
    sliderS_MAX->connect("ValueChanged", [&](){
        if (changeSlider){
            calibration->changedColor = true;
            calibration->range[S_MAX] = sliderS_MAX->getValue();
            calibration->updateSlider();
        }
    });

    sliderV_MAX = theme->load("Slider");
    gui.add(sliderV_MAX, "sliderV_MAX");
    sliderV_MAX->setPosition(int(halfScreen.x - calibration->imageSizeZoom.x/8 + calibration->imageSizeZoom.x/3), screen.y - 100 - 45);
    sliderV_MAX->setSize(calibration->imageSizeZoom.x/4, 12);
    sliderV_MAX->setMaximum(sliderMax[V]);
    sliderV_MAX->setMinimum(0);
    sliderV_MAX->setValue(calibration->range[V_MAX]);
    sliderV_MAX->connect("ValueChanged", [&](){
        if (changeSlider){
            calibration->changedColor = true;
            calibration->range[V_MAX] = sliderV_MAX->getValue();
            calibration->updateSlider();
        }
    }); 

    sliderH_MIN = theme->load("Slider");
    gui.add(sliderH_MIN, "sliderH_MIN");
    sliderH_MIN->setPosition(int(halfScreen.x - calibration->imageSizeZoom.x/8 - calibration->imageSizeZoom.x/3), screen.y - 100 - 45 + 20);
    sliderH_MIN->setSize(calibration->imageSizeZoom.x/4, 12);
    sliderH_MIN->setMaximum(sliderMax[H]);
    sliderH_MIN->setMinimum(0);
    sliderH_MIN->setValue(calibration->range[H_MIN]);
    sliderH_MIN->connect("ValueChanged", [&](){
        if (changeSlider){
            calibration->changedColor = true;
            calibration->range[H_MIN] = sliderH_MIN->getValue();
            calibration->updateSlider();
        }
    });    

    sliderS_MIN = theme->load("Slider");
    gui.add(sliderS_MIN, "sliderS_MIN");
    sliderS_MIN->setPosition(int(halfScreen.x - calibration->imageSizeZoom.x/8), screen.y - 100 - 45 + 20);
    sliderS_MIN->setSize(calibration->imageSizeZoom.x/4, 12);
    sliderS_MIN->setMaximum(sliderMax[S]);
    sliderS_MIN->setMinimum(0);
    sliderS_MIN->setValue(calibration->range[S_MIN]);
    sliderS_MIN->connect("ValueChanged", [&](){
        if (changeSlider){
            calibration->changedColor = true;
            calibration->range[S_MIN] = sliderS_MIN->getValue();
            calibration->updateSlider();
        }
    });

    sliderV_MIN = theme->load("Slider");
    gui.add(sliderV_MIN, "sliderV_MIN");
    sliderV_MIN->setPosition(int(halfScreen.x - calibration->imageSizeZoom.x/8 + calibration->imageSizeZoom.x/3), screen.y - 100 - 45 + 20);
    sliderV_MIN->setSize(calibration->imageSizeZoom.x/4, 12);
    sliderV_MIN->setMaximum(sliderMax[V]);
    sliderV_MIN->setMinimum(0);
    sliderV_MIN->setValue(calibration->range[V_MIN]);
    sliderV_MIN->connect("ValueChanged", [&](){
        if (changeSlider){
            calibration->changedColor = true;
            calibration->range[V_MIN] = sliderV_MIN->getValue();
            calibration->updateSlider();
        }
    }); 

    tab = std::make_shared<tgui::Tab>();
    gui.add(tab, "tab");
    tab->setFont(fontTitlePath);
    tab->setTextSize(16);
    tab->add(" Player 0 ");
    tab->add(" Player 1 ");
    tab->add(" Player 2 ");
    tab->add("   Team   ");
    tab->add(" Opponent ");
    tab->add("   Ball   ");
    tab->select(0);
    tab->setPosition(halfScreen.x - tab->getSize().x/2, screen.y - 105);
    tab->getRenderer()->setBackgroundColor(sf::Color::Transparent);
    tab->getRenderer()->setSelectedBackgroundColor(sf::Color::Transparent);    
    tab->getRenderer()->setTextColor(sf::Color::White);
    tab->getRenderer()->setSelectedTextColor(sf::Color::Red);
    tab->connect("TabSelected", [&](){ 
        calibration->selectedTab = tab->getSelectedIndex();
        calibration->updateTab();
        calibration->changedColor = false;
        setSlider(  calibration->range[H_MAX], calibration->range[S_MAX], calibration->range[V_MAX],
                    calibration->range[H_MIN], calibration->range[S_MIN], calibration->range[V_MIN]);
    });

    radioButton1 = theme->load("RadioButton");
    gui.add(radioButton1, "radioButton1");
    radioButton1->setFont(fontTitlePath);
    radioButton1->setText("Camera Image");
    radioButton1->setTextSize(13);
    radioButton1->setSize(15,15);
    radioButton1->setPosition(halfScreen.x-(tab->getSize().x/2)-(radioButton1->getFullSize().x)-100, screen.y - 100);
    radioButton1->connect("Checked", [&](){
        if (changeRadioButton){
            calibration->cameraOn = true;
            calibration->imageInitialize();
            calibration->pointCutFieldDefault();
        }
    });

    radioButton2 = theme->load("RadioButton");
    gui.add(radioButton2, "radioButton2");
    radioButton2->setFont(fontTitlePath);
    radioButton2->setText("Saved Image");
    radioButton2->setTextSize(13);
    radioButton2->setSize(15,15);
    radioButton2->setPosition(halfScreen.x+(tab->getSize().x/2)+100, screen.y - 100);
    radioButton2->connect("Checked", [&](){
        if (changeRadioButton){
            calibration->cameraOn = false;
            calibration->imageInitialize();
            calibration->pointCutFieldDefault();
        }
    });    

    if(calibration->cameraOn) { 
        changeRadioButton = false;
            radioButton1->check(); 
        changeRadioButton = true;
    
    } else { 
        changeRadioButton = false;
            radioButton2->check();
        changeRadioButton = true;
    }
}   

/*
 * Method for interface create using SFML library
 */
void GUICalibration::interfaceCreateSFML(){

    rodetasTitle.setFont(fontTitle);
    rodetasTitle.setCharacterSize(60);
    rodetasTitle.setColor(sf::Color(255,255,255));
    rodetasTitle.setString("Rodetas - Calibration");
    rodetasTitle.setPosition(int(halfScreen.x - (rodetasTitle.getLocalBounds().width/2)), 0);

    rotationText.setFont(fontTitle);
    rotationText.setCharacterSize(13);
    rotationText.setColor(sf::Color(255,255,255));
    rotationText.setString("Rotation");
    rotationText.setPosition(int(halfScreen.x - (rotationText.getLocalBounds().width/2)), screen.y - 100 - 80);

    rangeHText.setFont(fontTitle);
    rangeHText.setCharacterSize(13);
    rangeHText.setColor(sf::Color(255,255,255));
    rangeHText.setString("Range H ");
    rangeHText.setPosition(int(halfScreen.x - (calibration->imageSizeZoom.x/3) - (rangeHText.getLocalBounds().width/2)), screen.y - 100 - 35);

    rangeSText.setFont(fontTitle);
    rangeSText.setCharacterSize(13);
    rangeSText.setColor(sf::Color(255,255,255));
    rangeSText.setString("Range S");
    rangeSText.setPosition(int(halfScreen.x - (rangeSText.getLocalBounds().width/2)), screen.y - 100 - 35);

    rangeVText.setFont(fontTitle);
    rangeVText.setCharacterSize(13);
    rangeVText.setColor(sf::Color(255,255,255));
    rangeVText.setString("Range V");
    rangeVText.setPosition(int(halfScreen.x + (calibration->imageSizeZoom.x/3) - (rangeVText.getLocalBounds().width/2)), screen.y - 100 - 35);
    
    cutField.setFillColor(sf::Color::Transparent);
    cutField.setOutlineThickness(2);
    cutField.setOutlineColor(sf::Color(255,0,0));
    cutField.setSize(sf::Vector2f(pixelCut2.x-pixelCut1.x, pixelCut2.y-pixelCut1.y));

    gridView1.setSize(sf::Vector2f(calibration->imageSizeZoom.x, 2));
    gridView1.setPosition(sf::Vector2f(calibration->margin.x, calibration->margin.y + (calibration->imageSizeZoom.y/10)));
    gridView1.setFillColor(sf::Color(145,145,145));

    gridView2.setSize(sf::Vector2f(calibration->imageSizeZoom.x, 2));
    gridView2.setPosition(sf::Vector2f(calibration->margin.x, calibration->margin.y + calibration->imageSizeZoom.y - (calibration->imageSizeZoom.y/10)));
    gridView2.setFillColor(sf::Color(145,145,145));

    gridView3.setSize(sf::Vector2f(2, calibration->imageSizeZoom.y));
    gridView3.setPosition(sf::Vector2f(calibration->margin.x + calibration->imageSizeZoom.x - (calibration->imageSizeZoom.x/10), calibration->margin.y));
    gridView3.setFillColor(sf::Color(145,145,145));

    gridView4.setSize(sf::Vector2f(2, calibration->imageSizeZoom.y));
    gridView4.setPosition(sf::Vector2f(calibration->margin.x + (calibration->imageSizeZoom.x/10), calibration->margin.y));
    gridView4.setFillColor(sf::Color(145,145,145));
}

/*
 * Method for drawing the field 
 */
void GUICalibration::fieldCreate(){
    Point convexPosition;
    convexPosition.x = calibration->margin.x;
    convexPosition.y = calibration->margin.y + calibration->imageSizeZoom.y/2;

    Point goal = calibration->goal;

    fieldConvexShape.setPointCount(13);
    fieldConvexShape.setPoint(0, sf::Vector2f(convexPosition.x,convexPosition.y));
    fieldConvexShape.setPoint(1, sf::Vector2f(convexPosition.x,convexPosition.y-(goal.y/2)));
    fieldConvexShape.setPoint(2, sf::Vector2f(convexPosition.x+goal.x,convexPosition.y-(goal.y/2)));
    fieldConvexShape.setPoint(3, sf::Vector2f(convexPosition.x+goal.x,convexPosition.y-(calibration->imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(4, sf::Vector2f(convexPosition.x+calibration->imageSizeZoom.x-goal.x,convexPosition.y-(calibration->imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(5, sf::Vector2f(convexPosition.x+calibration->imageSizeZoom.x-goal.x,convexPosition.y-(goal.y/2)));
    fieldConvexShape.setPoint(6, sf::Vector2f(convexPosition.x+calibration->imageSizeZoom.x,convexPosition.y-(goal.y/2)));
    fieldConvexShape.setPoint(7, sf::Vector2f(convexPosition.x+calibration->imageSizeZoom.x,convexPosition.y+(goal.y/2)));
    fieldConvexShape.setPoint(8, sf::Vector2f(convexPosition.x+calibration->imageSizeZoom.x-goal.x,convexPosition.y+(goal.y/2)));
    fieldConvexShape.setPoint(9, sf::Vector2f(convexPosition.x+calibration->imageSizeZoom.x-goal.x,convexPosition.y+(calibration->imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(10, sf::Vector2f(convexPosition.x+goal.x,convexPosition.y+(calibration->imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(11, sf::Vector2f(convexPosition.x+goal.x,convexPosition.y+(goal.y/2)));
    fieldConvexShape.setPoint(12, sf::Vector2f(convexPosition.x,convexPosition.y+(goal.y/2)));
}

/*
 * Getters & Setters
 */ 

void GUICalibration::setSlider(int h_max, int s_max, int v_max, int h_min, int s_min, int v_min){
    // update slider exists  because of tgui, that recognizes setValue like a connection
    changeSlider = false; 
        sliderH_MAX->setValue(h_max);
        sliderS_MAX->setValue(s_max);
        sliderV_MAX->setValue(v_max);
        sliderH_MIN->setValue(h_min);
        sliderS_MIN->setValue(s_min);
        sliderV_MIN->setValue(v_min);
    changeSlider = true;

    calibration->range[H_MAX] = h_max;
    calibration->range[S_MAX] = s_max;
    calibration->range[V_MAX] = v_max;
    calibration->range[H_MIN] = h_min;
    calibration->range[S_MIN] = s_min;
    calibration->range[V_MIN] = v_min;
    calibration->updateSlider();
}

void GUICalibration::setGUI(Calibration *c){
    calibration = c;
}