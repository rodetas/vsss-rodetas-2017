#include "Simulator.h"

Simulator::Simulator(){
    moveRobot = false;
    moveBall = false;
    endSimulator = false;
    ballCreated = false;
    simulatorOpened = false;
    atualRobot = 0;
    information.resize(3);
    field.resize(4);
    objects.resize(7);
    textRobots.resize(7);
    textScreen.resize(5);
    targets.resize(3);

    for(int i=0 ; i<3 ; i++){
        targets[i].setRadius(5);
        targets[i].setOrigin(5/2, 5/2);
    }

    targets[0].setFillColor(sf::Color(255,51,204));
    targets[1].setFillColor(sf::Color(0,255,0));
    targets[2].setFillColor(sf::Color(184,0,138));

}

void Simulator::initialize(){
    rateZoom = 0.7;

    screenSize.x    = sf::VideoMode::getDesktopMode().width;
    screenSize.y    = sf::VideoMode::getDesktopMode().height;
    halfScreen.x    = screenSize.x/2;
    halfScreen.y    = screenSize.y/2;
    imageSizeZoom.x = screenSize.x*rateZoom;
    imageSizeZoom.y = screenSize.y*rateZoom;
    goal.y          = (imageSizeZoom.y/2)*rateZoom;
    goal.x          = goal.y/3;
    origin.x        = halfScreen.x - imageSizeZoom.x/2;
    origin.y        = halfScreen.y - imageSizeZoom.y/2;
    sizeRobot       = imageSizeZoom.x*0.05;
    radiusBall      = imageSizeZoom.x*0.01;
}

void Simulator::simulate(){
       
    if (!simulatorOpened){
        initialize();
        loadTextures();
        fieldCreate();
        textsCreate();
        simulatorOpened = true;
    }
    events();
    drawWindow();
}

void Simulator::openWindow(){
    window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, sf::VideoMode::getDesktopMode().bitsPerPixel), "Rodetas Simulator", sf::Style::Titlebar | sf::Style::Close);
}

void Simulator::loadTextures(){
	sf::Texture texture;
	string pathImage = "files/textures/";
	string typeImage = ".png";
	string textureLocal;

	for (int i = 1; i < 7; i++){	
		textureLocal = pathImage + to_string(i) + typeImage;
		texture.loadFromFile(textureLocal);
		texture.setSmooth(true);

		texturesRobots.push_back(texture);
	}
    background.loadFromFile("files/textures/background.jpg");
    textureBall.loadFromFile("files/textures/ball.png");
}

bool Simulator::onBall(sf::CircleShape c, int x, int y){
    return (sqrt ( pow((c.getPosition().x - x),2) + pow((c.getPosition().y - y),2) ) <= c.getRadius());
}

bool Simulator::onField(int x, int y){
    return (field[fieldLine].getGlobalBounds().contains(x,y) ||
            field[goal1Line].getGlobalBounds().contains(x,y) ||
            field[goal2Line].getGlobalBounds().contains(x,y)  );
}

void Simulator::robotCreate(int x, int y){
    sf::RectangleShape r;
    r.setSize(sf::Vector2f(sizeRobot, sizeRobot));
    r.setOrigin(sf::Vector2f(sizeRobot/2, sizeRobot/2));
    r.setTexture(&texturesRobots[robots.size()]);
    r.setPosition(x, y);
    robots.push_back(r);

}

void Simulator::ballCreate(int x, int y){
    ball.setRadius(radiusBall);
    ball.setOrigin(sf::Vector2f(radiusBall, radiusBall));
    ball.setTexture(&textureBall);
    ball.setPosition(x, y);
}

void Simulator::textsCreate(){
  
    fontTitle.loadFromFile(fontTitlePath);
    fontText.loadFromFile(fontTextPath);

    for (int i = 0; i < textRobots.size(); i++){
        int distanceBetweenTexts = rateZoom*30*(i);

        textRobots[i].setFont(fontText);
        textRobots[i].setCharacterSize(rateZoom*20);
        textRobots[i].setColor(sf::Color(255,255,255));
        textRobots[i].setPosition(int(halfScreen.x - imageSizeZoom.x/2 + goal.x), int(halfScreen.y - imageSizeZoom.y/2 + distanceBetweenTexts));
        
        if (i < 3){
            textRobots[i].setString( "Robot Team " + to_string(i) + ": {0,0} (0)");
        } else {
            textRobots[i].setString( "Robot Oponnent " + to_string(i-2) + ": {0,0}");
        }
    }
    textRobots[graphicBall].setString("Ball: {0,0}");

    for (int i = 0; i < textScreen.size(); i++){
        int distanceBetweenTexts = rateZoom*30*(abs(i-1));

        textScreen[i].setString("-----------------------------------------------------");
        textScreen[i].setFont(fontText);
        textScreen[i].setCharacterSize(rateZoom*20);
        textScreen[i].setColor(sf::Color(255,255,255));
        textScreen[i].setPosition(int(halfScreen.x + imageSizeZoom.x/2 - goal.x - (textScreen[i].getLocalBounds().width)), int(halfScreen.y - imageSizeZoom.y/2 + distanceBetweenTexts));
    }

    textScreen[fpsInformation].setString("Fps: 100");
    textScreen[fpsInformation].setPosition(int(halfScreen.x + imageSizeZoom.x/2 - goal.x - (textScreen[fpsInformation].getLocalBounds().width)), int(halfScreen.y - imageSizeZoom.y/2));

    textScreen[titleInformation].setFont(fontTitle);
    textScreen[titleInformation].setCharacterSize(rateZoom*80);
    textScreen[titleInformation].setString("Rodetas - Simulator");
    textScreen[titleInformation].setPosition(int(halfScreen.x - (textScreen[titleInformation].getLocalBounds().width/2)), 0);
}

void Simulator::fieldCreate(){
    //center at the midle of the field
    fieldConvexShape.setPointCount(13);
    fieldConvexShape.setPoint(0,  sf::Vector2f(0,0));
    fieldConvexShape.setPoint(1,  sf::Vector2f(0, -(goal.y/2)));
    fieldConvexShape.setPoint(12, sf::Vector2f(0,  (goal.y/2)));
    fieldConvexShape.setPoint(2,  sf::Vector2f(goal.x, -(goal.y/2)));
    fieldConvexShape.setPoint(11, sf::Vector2f(goal.x,  (goal.y/2)));
    fieldConvexShape.setPoint(3,  sf::Vector2f(goal.x, -(imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(10, sf::Vector2f(goal.x,  (imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(4,  sf::Vector2f(imageSizeZoom.x-goal.x, -(imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(9,  sf::Vector2f(imageSizeZoom.x-goal.x, (imageSizeZoom.y/2)));
    fieldConvexShape.setPoint(5,  sf::Vector2f(imageSizeZoom.x-goal.x, -(goal.y/2)));
    fieldConvexShape.setPoint(8,  sf::Vector2f(imageSizeZoom.x-goal.x, (goal.y/2)));
    fieldConvexShape.setPoint(6,  sf::Vector2f(imageSizeZoom.x, -(goal.y/2)));
    fieldConvexShape.setPoint(7,  sf::Vector2f(imageSizeZoom.x,  (goal.y/2)));
    fieldConvexShape.setOrigin(imageSizeZoom.x/2, 0);
    fieldConvexShape.setTexture(&background);
    fieldConvexShape.setPosition(halfScreen.x, halfScreen.y);

    field[fieldLine].setSize(sf::Vector2f(imageSizeZoom.x - (2*goal.x), imageSizeZoom.y));
    field[midleLine].setSize(sf::Vector2f(0, imageSizeZoom.y));
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
    field[goal1Line].setPosition(fieldConvexShape.getPosition().x - (imageSizeZoom.x/2) + (goal.x/2) - 2, fieldConvexShape.getPosition().y);
    field[goal2Line].setPosition(fieldConvexShape.getPosition().x + (imageSizeZoom.x/2) - (goal.x/2) + 2, fieldConvexShape.getPosition().y);
}

void Simulator::events(){

    sf::Event event;
    if (window.pollEvent(event)){

        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)){
            window.close();
            endSimulator = true;

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B){
            if (!ballCreated)
                ballCreate(halfScreen.x, halfScreen.y);
        
        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R){
            if (robots.size() < 6){
                robotCreate(500, 400);
            }

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D){
            if (robots.size() > 0){
                robots.pop_back();
            }

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left){
            robots[atualRobot].setRotation(int(robots[atualRobot].getRotation())-5);

        } else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right){
            robots[atualRobot].setRotation(int(robots[atualRobot].getRotation())+5);
        
        } else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
            moveRobot = false;
            moveBall = false;

        } else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){

            positionMouse = sf::Mouse::getPosition(window);

            if (moveBall && !moveRobot && onField(positionMouse.x, positionMouse.y)){
                ball.setPosition(positionMouse.x, positionMouse.y);

            } else if (moveRobot && onField(positionMouse.x, positionMouse.y)){
                robots[atualRobot].setPosition(positionMouse.x, positionMouse.y);

            } else if(onBall(ball, positionMouse.x, positionMouse.y)){
                moveBall = true;    

            } else {
                for (atualRobot = 0; atualRobot < robots.size(); atualRobot++){
                    if (robots[atualRobot].getGlobalBounds().contains(positionMouse.x, positionMouse.y)) {
                        moveRobot = true;
                        break;
                    }
                }
            }            
        }
    }
}

void Simulator::drawWindow(){

    window.clear(sf::Color(0, 0, 0));

    window.draw(fieldConvexShape);
    for (int i = 0; i < field.size(); i++){
        window.draw(field[i]);
    }
    
    textRobots[graphicBall].setString( "Ball: { " + to_string(int(ball.getPosition().x - origin.x)) + " , " + to_string(int(ball.getPosition().y - origin.y)) + " }" );
    window.draw(ball);

    for (int i = 0; i < robots.size(); i++){        
        window.draw(robots[i]);
        if (i < 3){
            textRobots[i].setString( "Robot Team " + to_string(i) + ": { " + to_string(int(robots[i].getPosition().x - origin.x)) + " , " + to_string(int(robots[i].getPosition().y - origin.y)) + " } (" + to_string(int(robots[i].getRotation())) + ")" );
        } else {
            textRobots[i].setString( "Robot Oponnent " + to_string(i-2) + ": { " + to_string(int(robots[i].getPosition().x)) + " , " + to_string(int(robots[i].getPosition().y)) + " } ");
        }
    }

    for(int i=0 ; i<targets.size() ; i++){
        window.draw(targets[i]);
    }

    for (int i = 0; i < textRobots.size(); i++){
        window.draw(textRobots[i]);
    }

    for (int i = 0; i < 3; i++){
        textScreen[i+2].setString(information[i]);
    }
    textScreen[fpsInformation].setString("Fps: " + to_string(int(fps.framesPerSecond())));

    for (int i = 0; i < textScreen.size(); i++){
        window.draw(textScreen[i]);
    }

    window.display();
}


/*
 * Getters
 */
bool Simulator::getEndSimulator(){
    return this->endSimulator;
}

vector<Object> Simulator::getPositions(){
    for (int i = 0; i < robots.size(); i++){
        objects[i].x = robots[i].getPosition().x - origin.x;
        objects[i].y = robots[i].getPosition().y - origin.y;
        objects[i].angle = robots[i].getRotation();
    }

    objects[graphicBall].x = ball.getPosition().x - origin.x;
    objects[graphicBall].y = ball.getPosition().y - origin.y;

    return objects;
}

/*
 * Setters
 */
void Simulator::setEndSimulator(bool simulator){
    this->endSimulator = simulator;
}

void Simulator::setInformation(string* v){
    for(int i=0 ; i<information.size() ; i++){
        information[i] = v[i];
    }

}

void Simulator::setTargets(vector<Point> t){
    for(int i=0 ; i<t.size() ; i++){
        targets[i].setPosition(t[i].x + origin.x, t[i].y + origin.y);
    }
}  