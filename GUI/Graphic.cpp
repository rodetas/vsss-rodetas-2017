#include "Graphic.h"

/*
 * class constructor
 */
Graphic::Graphic(){

    colorsHSV.resize(6);
    colorsRGB.resize(6);
    objects.resize(7);
    robot.resize(6);
    field.resize(4);
    textScreen.resize(4);
    targets.resize(3);
    buttons.resize(9);

    rotateField = false;
    power = 1.3;
    powerCurve = 0.6;
    menu = false;
    close = false;
    pause = true;
    logConsoleOpen = false;
    screenPercentage = 0.7;
    savingPositions = false;
    testMovement = STOPPED_MOVE;
}

/*
 * Method to initialize graphic
 *      Variables:
 *          rateFieldSize - ratio to multiply the size of element to adjust on the screen
 *          origin - point of the origin to draw the robots
 */
void Graphic::initialize(){
    manipulation.loadCalibration();

    colorsHSV       = manipulation.getColorsHsv();
    colorsRGB       = manipulation.getColorsRgb();
    imageSize.x     = abs( manipulation.getPointField2().x - manipulation.getPointField1().x );
    imageSize.y     = abs( manipulation.getPointField2().y - manipulation.getPointField1().y );
    screenSize.x    = sf::VideoMode::getDesktopMode().width;
    screenSize.y    = sf::VideoMode::getDesktopMode().height;
    viewSize.x      = screenSize.x;
    viewSize.y      = screenSize.y;
    halfScreen.x    = screenSize.x/2;
    halfScreen.y    = screenSize.y/2;
    rateFieldSize	= (screenSize.y * screenPercentage)/imageSize.y;
    fieldSize.x 	= imageSize.x * rateFieldSize;
    fieldSize.y 	= imageSize.y * rateFieldSize;
    goal.x          = manipulation.getGoal().x * rateFieldSize;
    goal.y          = manipulation.getGoal().y * rateFieldSize;
    origin.x        = halfScreen.x - fieldSize.x/2;
    origin.y        = halfScreen.y - fieldSize.y/2;
    sizeRobot       = fieldSize.y*0.007;
    radiusBall      = fieldSize.y*0.015;

    interfaceCreateSFML();
    interfaceCreateTGUI();
    initializeOpenGL(screenSize, viewSize, sizeRobot);
    interfaceCreateButton();
}

/*
 * method to set the position of the robots and ball, and update the informations
 */
void Graphic::gameInformation(){

    for (int i = 0; i < robot.size(); i++){
        robot[i].x = objects[i].x;
        robot[i].y = objects[i].y;
        robot[i].angle = - objects[i].angle + 180;
    }

    drawBall.setPosition( objects[graphicBall].x, objects[graphicBall].y);
    textScreen[fpsInformation].setString("Fps: " + to_string(fpsCount));

    if(connectionStatus){
        textScreen[connectionStatusInformation].setString("");
    } else {
        textScreen[connectionStatusInformation].setString("TRANSMISSION FAILS");
    }

    events();
    windowDraw();

    if(savingPositions){

        if(clockVector.getElapsedTime().asMilliseconds() > 500){ //posicoes de meio em meio segundo
            lastObjects.insert(lastObjects.begin(), objects.begin(), objects.end());
            clockVector.restart();
        }

        //cout << clock.getElapsedTime().asMilliseconds() << endl;
        if(clockSave.getElapsedTime().asMilliseconds() > 4000){ // salva no arquivo de quatro em quatro segundos
            saveCoordinates();
            lastObjects.clear();
            clockSave.restart();
        }
    }
}

/*
 * Method for events SFML
 */
void Graphic::events(){
    sf::Event event;

    while (window.pollEvent(event)){
        if (event.type == sf::Event::Closed){
            window.close();
            menu = true;
        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
            window.close();
            close = true;
        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){
            if(pause){
                pause = false;
                textScreen[gameStatusInformation].setString("Playing");
            } else if(!pause){
                pause = true;
                textScreen[gameStatusInformation].setString("Pause");
            }

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
            testMovement = LEFT_MOVE;

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
            testMovement = RIGHT_MOVE;

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
            testMovement = FORWARD_MOVE;

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
            testMovement = BACK_MOVE;

        } else if(buttons[0].buttonEvent(event)){ //play game
            pause = false;
            textScreen[gameStatusInformation].setString("Playing");

        } else if(buttons[1].buttonEvent(event)){ //pause game
            pause = true;
            textScreen[gameStatusInformation].setString("Pause");

        } else if(buttons[2].buttonEvent(event)){ // rotate camera
            rotateField = !rotateField;

        } else if(buttons[3].buttonEvent(event)){ //penalty
            penalti = true;

        } else if(buttons[4].buttonEvent(event)){ //show log console
            logConsole();

        } else if(buttons[5].buttonEvent(event)){ //menu button
            menu = true;
            window.close();

        } else if(buttons[6].buttonEvent(event)){
            tgui::EditBox::Ptr editBox = gui.get<tgui::EditBox>("editBox");
            sf::String text = editBox->getText();
            istringstream iss(text);
            iss >> power;

        } else if(buttons[7].buttonEvent(event)){
            tgui::EditBox::Ptr editBox1 = gui.get<tgui::EditBox>("editBox1");
            sf::String text = editBox1->getText();
            istringstream iss(text);
            iss >> powerCurve;

        } else if(buttons[8].buttonEvent(event)){
            tgui::EditBox::Ptr editBox2 = gui.get<tgui::EditBox>("editBox2");
            sf::String text = editBox2->getText();
            savingPositions = !savingPositions;
            manipulation.setFileName(text);

        } else {
            testMovement = STOPPED_MOVE;
        }

        gui.handleEvent(event);
    }
}

/*
 * draw GUI, interface objects
 */
void Graphic::windowDraw(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window.pushGLStates();

        gui.draw();

        window.draw(fieldConvexShape);

        for (int i = 0; i < field.size(); i++){
            window.draw(field[i]);
        }

        for (int i = 0; i < buttons.size() ; i++){
            window.draw(buttons[i]);
        }

        for (int i = 0; i < textScreen.size(); i++){
            window.draw(textScreen[i]);
        }

        cornerTest();

    window.popGLStates();

    for (int i = 0; i < robot.size(); i++){
        if(robot[i].x > 0 && robot[i].y > 0)
            drawRobotOpenGL(robot[i]);
    }

    window.pushGLStates();

        if(objects[graphicBall].x > 0 && objects[graphicBall].y > 0)
                window.draw(drawBall);

        for(int i=0 ; i<3 ; i++){
            window.draw(targets[i]);
        }    

    window.popGLStates();

    window.display();

    if (logConsoleOpen){
        updateLogConsole();
    }
}

/*
 * draws lines on the image to test the distance of the corners
 */
void Graphic::cornerTest(){
    top.setSize(sf::Vector2f(fieldSize.x, 2));
    top.setPosition(sf::Vector2f(origin.x, origin.y + fieldSize.y * 0.9));
    top.setFillColor(sf::Color::Red);

    under.setSize(sf::Vector2f(fieldSize.x, 2));
    under.setPosition(sf::Vector2f(origin.x, origin.y + fieldSize.y * 0.1));
    under.setFillColor(sf::Color::Red);

    left.setSize(sf::Vector2f(2, fieldSize.y));
    left.setPosition(sf::Vector2f(origin.x+fieldSize.x * 0.15 , origin.y));
    left.setFillColor(sf::Color::Red);

    right.setSize(sf::Vector2f(2, fieldSize.y));
    right.setPosition(sf::Vector2f(origin.x+fieldSize.x * 0.85, origin.y));
    right.setFillColor(sf::Color::Red);

    window.draw(top);
    window.draw(under);
    window.draw(right);
    window.draw(left);
}

/*
 * load the textures for robots, ball, background
 */
void Graphic::loadTextures(){
    background.loadFromFile(backgroundPath);
    textureBall.loadFromFile(textureBallPath);
}

/*
 * create texts on the screen
 */
void Graphic::textsCreate(){

    fontTitle.loadFromFile(fontTitlePath);
    fontText.loadFromFile(fontTextPath);

    for (int i = 0; i < textScreen.size(); i++){
        textScreen[i].setFont(fontText);
        textScreen[i].setCharacterSize(screenSize.y * 0.02);
    }

    textScreen[fpsInformation].setString("Fps: 100");
    textScreen[gameStatusInformation].setString("Pause");
    textScreen[fpsInformation].setPosition(int(halfScreen.x + fieldSize.x/2 - goal.x - (textScreen[fpsInformation].getLocalBounds().width)), int(halfScreen.y - fieldSize.y/2));
    textScreen[gameStatusInformation].setPosition(int(halfScreen.x + fieldSize.x/2 - goal.x - (textScreen[fpsInformation].getLocalBounds().width)), int(halfScreen.y - fieldSize.y/2 + (textScreen[fpsInformation].getLocalBounds().height)));

    textScreen[titleInformation].setFont(fontTitle);
    textScreen[titleInformation].setCharacterSize(screenSize.y * 0.08);

    textScreen[titleInformation].setString("Rodetas");
    textScreen[titleInformation].setPosition(int(halfScreen.x - (textScreen[titleInformation].getLocalBounds().width/2)), 0);

    textScreen[connectionStatusInformation].setPosition(int(fieldSize.x), int(fieldSize.y));
    textScreen[connectionStatusInformation].setString("OK");
}

/*
 * create field on the screen
 */
void Graphic::fieldCreate(){

    // center at the midle of the field
    fieldConvexShape.setPointCount(13);
    fieldConvexShape.setPoint(0,  sf::Vector2f(0                        , 0));
    fieldConvexShape.setPoint(1,  sf::Vector2f(0                        , -(goal.y/2)));
    fieldConvexShape.setPoint(2,  sf::Vector2f(goal.x                   , -(goal.y/2)));
    fieldConvexShape.setPoint(3,  sf::Vector2f(goal.x                   , -(fieldSize.y/2)));
    fieldConvexShape.setPoint(4,  sf::Vector2f(fieldSize.x-goal.x   , -(fieldSize.y/2)));
    fieldConvexShape.setPoint(5,  sf::Vector2f(fieldSize.x-goal.x   , -(goal.y/2)));
    fieldConvexShape.setPoint(6,  sf::Vector2f(fieldSize.x          , -(goal.y/2)));
    fieldConvexShape.setPoint(7,  sf::Vector2f(fieldSize.x          , (goal.y/2)));
    fieldConvexShape.setPoint(8,  sf::Vector2f(fieldSize.x-goal.x   , (goal.y/2)));
    fieldConvexShape.setPoint(9,  sf::Vector2f(fieldSize.x-goal.x   , (fieldSize.y/2)));
    fieldConvexShape.setPoint(10, sf::Vector2f(goal.x                   , (fieldSize.y/2)));
    fieldConvexShape.setPoint(11, sf::Vector2f(goal.x                   , (goal.y/2)));
    fieldConvexShape.setPoint(12, sf::Vector2f(0                        , (goal.y/2)));
    fieldConvexShape.setOrigin(fieldSize.x/2, 0);
    fieldConvexShape.setTexture(&background);
    fieldConvexShape.setPosition(halfScreen.x, halfScreen.y);

    field[fieldLine].setSize(sf::Vector2f(fieldSize.x - (2*goal.x), fieldSize.y));
    field[midleLine].setSize(sf::Vector2f(0, fieldSize.y));
    field[goal1Line].setSize(sf::Vector2f(goal.x-4,goal.y));
    field[goal2Line].setSize(sf::Vector2f(goal.x-4,goal.y));

    for (int i = 0; i < field.size() ; i++){
        field[i].setOutlineThickness(4);
        field[i].setOutlineColor(sf::Color::White);
        field[i].setFillColor(sf::Color::Transparent);
        field[i].setOrigin(field[i].getSize().x/2, field[i].getSize().y/2);
    }

    field[fieldLine].setPosition(sf::Vector2f(fieldConvexShape.getPosition().x, fieldConvexShape.getPosition().y));
    field[midleLine].setPosition(sf::Vector2f(fieldConvexShape.getPosition().x, fieldConvexShape.getPosition().y));
    field[goal1Line].setPosition(fieldConvexShape.getPosition().x - (fieldSize.x/2) + (goal.x/2) - 2, fieldConvexShape.getPosition().y);
    field[goal2Line].setPosition(fieldConvexShape.getPosition().x + (fieldSize.x/2) - (goal.x/2) + 2, fieldConvexShape.getPosition().y);
}

/*
 * Method for interface create using SFML library
 */
void Graphic::interfaceCreateSFML(){

    loadTextures();
    fieldCreate();
    textsCreate();

    // create team drawings
    for (int i = 0; i < 3; i++){
        robot[i].colorTeam = colorsRGB[team];
        robot[i].colorPlayer = colorsRGB[i];
    }

    // create opponent drawings
    for (int i = 3; i < robot.size(); i++){
        robot[i].colorTeam = colorsRGB[opponent];
        robot[i].colorPlayer = colorsRGB[opponent];
    }

    for(int i=0 ; i<3 ; i++){
        targets[i].setRadius(5);
        targets[i].setOrigin(5/2, 5/2);
        targets[i].setFillColor(sf::Color(colorsRGB[i].r, colorsRGB[i].g, colorsRGB[i].b));
    }

    drawBall.setRadius(radiusBall);
    drawBall.setTexture(&textureBall);
    drawBall.setOrigin(radiusBall/2, radiusBall/2);

    window.create(sf::VideoMode(screenSize.x, screenSize.y, sf::VideoMode::getDesktopMode().bitsPerPixel), "Rodetas", sf::Style::Titlebar | sf::Style::Close);

    #ifdef FPS
        window.setFramerateLimit(FPS);
    #endif

}

/*
 * Method for interface create using TGUI library
 */
void Graphic::interfaceCreateTGUI(){

    gui.setWindow(window);

    tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>(tguiThemePath);

    tgui::EditBox::Ptr editBox = theme->load("EditBox");
    gui.add(editBox, "editBox");
    editBox->setSize(100, 40);
    editBox->setPosition(halfScreen.x + fieldSize.x/2 + 70, origin.y + 40);
    editBox->setTextSize(15);
    stringstream pot; pot << power; string powerString = pot.str();
    editBox->setText(powerString);

    tgui::EditBox::Ptr editBox1 = theme->load("EditBox");
    gui.add(editBox1, "editBox1");
    editBox1->setSize(100, 40);
    editBox1->setPosition(halfScreen.x + fieldSize.x/2 + 70, origin.y + 150);
    editBox1->setTextSize(15);
    stringstream cc; cc << powerCurve; string curveString = cc.str();
    editBox1->setText(curveString);

    tgui::EditBox::Ptr editBox2 = theme->load("EditBox");
    gui.add(editBox2, "editBox2");
    editBox2->setSize(100, 40);
    editBox2->setPosition(halfScreen.x + fieldSize.x/2 + 70, origin.y + 260);
    editBox2->setTextSize(15);
}

void Graphic::interfaceCreateButton(){

    for (int i = 0; i < buttons.size(); i++){
        buttons[i].rectangleSize(100,40);
    }

    buttons[0].setPosition(origin.x - 170 + 50, origin.y + 20);
    buttons[0].textButton("Start", 15);

    buttons[1].setPosition(origin.x - 170 + 50, origin.y + 70 + 20);
    buttons[1].textButton("Pause", 15);

    buttons[2].setPosition(origin.x - 170 + 50, origin.y + 140 + 20);
    buttons[2].textButton("Side", 15);

    buttons[3].setPosition(origin.x - 170 + 50, origin.y + 210 + 20);
    buttons[3].textButton("Penalty", 15);

    buttons[4].setPosition(origin.x - 170 + 50, origin.y + 280 + 20);
    buttons[4].textButton("Log", 15);

    buttons[5].setPosition(origin.x - 170 + 50, origin.y + 350 + 20);
    buttons[5].textButton("Menu", 15);

    buttons[6].setPosition(halfScreen.x + fieldSize.x/2 + 70 + 50, origin.y + 20);
    buttons[6].textButton("Potency", 15);

    buttons[7].setPosition(halfScreen.x + fieldSize.x/2 + 70 + 50, origin.y + 20 + 110);
    buttons[7].textButton("Curve", 15);

    buttons[8].setPosition(halfScreen.x + fieldSize.x/2 + 70 + 50, origin.y + 20 + 220);
    buttons[8].textButton("Filename", 15);

}

void Graphic::saveCoordinates(){
    if(!pause) manipulation.saveCoordinates(lastObjects);
}

void Graphic::logConsole(){

    if (!logConsoleOpen){
        windowLogConsole.create(sf::VideoMode(screenSize.x/3, screenSize.y/2, sf::VideoMode::getDesktopMode().bitsPerPixel), "Log Console", sf::Style::Close);
        windowLogConsole.setPosition(sf::Vector2i(2*screenSize.x/3, screenSize.y/2));

        windowSizeLogConsole    = {windowLogConsole.getSize().x, windowLogConsole.getSize().y};

        openGLWindowLogConsole.initializeOpenGL(windowSizeLogConsole, windowSizeLogConsole, (windowSizeLogConsole.x * 0.025) );
    } else {
        windowLogConsole.close();
    }
        logConsoleOpen = !logConsoleOpen;
}

void Graphic::updateLogConsole(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float characterSize = windowSizeLogConsole.y * 0.04;

    for (int i = 0; i < information.size(); i++){
        vector<sf::Text> t(7);
        t[0].setString( "Robot " + toString(i, 2) );
        t[1].setString( information[i].strategy + " - " + information[i].move );
        t[2].setString( "PWM: " + toString(information[i].pwm1, 2) + " | " + toString(information[i].pwm2, 2) );
        t[3].setString( "Angle: " + toString(information[i].angle,2) );
        t[4].setString( "Point: {" + toString(information[i].x, 2) + " , " + toString(information[i].y, 2) + "}" );
        t[5].setString( "Sin: " + toString(information[i].sin, 2) );
        t[6].setString( "Cos: " + toString(information[i].cos, 2) );

        for (int j = 0; j < t.size(); j++){
            t[j].setPosition(int(windowSizeLogConsole.x * 0.3) , int(10 + (characterSize * j) + (characterSize * (t.size()+1) * i)) );
            t[j].setFont(fontText);
            t[j].setCharacterSize(characterSize);
            windowLogConsole.pushGLStates();
            windowLogConsole.draw(t[j]);
            windowLogConsole.popGLStates();
        }

        RobotOpenGL r = robot[i];
        r.x = (windowSizeLogConsole.x * 0.18);
        r.y = (windowSizeLogConsole.y * 0.20) + (windowSizeLogConsole.y * 0.3) * i;

        openGLWindowLogConsole.drawRobotOpenGL(r);
    }
    windowLogConsole.display();

	sf::Event eventLogConsole;
    while (windowLogConsole.pollEvent(eventLogConsole)){
		if (eventLogConsole.type == sf::Event::Closed){
    	    windowLogConsole.close();
			logConsoleOpen = !logConsoleOpen;
    	}
	}
}

/*
 * Method to convert the original coordinates from image to draw coordinates
 */
template <typename typePoint>
typePoint Graphic::changeCoordinate(typePoint point){
	point.x = float(point.x) * float(fieldSize.x) / float(imageSize.x) + origin.x;
	point.y = float(point.y) * float(fieldSize.y) / float(imageSize.y) + origin.y;

	return point;
}

/*
 * Setters
 */
void Graphic::setObjects(vector<Object> vec_objects){
	for (int i = 0; i < vec_objects.size(); i++){
	 	objects[i] = changeCoordinate(vec_objects[i]);
	}
}

void Graphic::setTargets(vector<Point> vec_target){
    for(int i=0 ; i<vec_target.size() ; i++){
        targets[i].setPosition(changeCoordinate(vec_target[i]).x, changeCoordinate(vec_target[i]).y);
    }
}

void Graphic::setFps(int fps){
    this->fpsCount = fps;
}

void Graphic::setMenu(bool b){
    this->menu = b;
}

void Graphic::setConnectionStatus(bool status){
    this->connectionStatus = status;
}

void Graphic::setInformation(vector<Information> vec_information){
    information = vec_information;
}


/*
 * Getters
 */
string Graphic::getTestMovement(){
    return testMovement;
}

float Graphic::getPowerCurve(){
    return powerCurve;
}

float Graphic::getPower(){
    return power;
}

bool Graphic::isPause(){
    return pause;
}

bool Graphic::isPenalti(){
    return penalti;
}

bool Graphic::getMenu(){
    return menu;
}

bool Graphic::getClose(){
    return close;
}

bool Graphic::getRotateField(){
    return rotateField;
}
