#include "Test.h"

Test::Test(){

}

int Test::loop(){

    // Create the main window

    window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, sf::VideoMode::getDesktopMode().bitsPerPixel), "SFML OpenGL");
    //window.create(sf::VideoMode(1000, 700, 32), "SFML OpenGL");

    view.x = 500; //window.getSize().x;
    view.y = 500; //window.getSize().y;

    Point2f screenSize = Point2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    Point2f viewSize = screenSize;
    Point2f imageSizeZoom = Point2f(600,400);
    Robot_Draw robot;

    opengl.initializeOpenGL(screenSize, viewSize, imageSizeZoom.y*0.01);

    float angle = 0;
    float x = 0, y = 0;

    Fps fps;

    while (window.isOpen()){

        

        cout << fps.framesPerSecond() << endl;

        sf::Event Event;
        while (window.pollEvent(Event)){
            if (Event.type == sf::Event::Closed)
                window.close();
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();                
            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Left)
                angle = angle + 5;
            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Right)
                angle = angle - 5;
            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Down)
                y--;
            if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Up)
                y++;
        }

    Rgb color_team;
        color_team.r = 0;
        color_team.g = 0;
        color_team.b = 255;

    Rgb color_player;
        color_player.r = 0;
        color_player.g = 255;
        color_player.b = 0;

        robot.color_team = (color_team);
        robot.color_player = (color_player);
        robot.angle = angle;
        robot.x =  sf::Mouse::getPosition(window).x;
        robot.y =  sf::Mouse::getPosition(window).y;

        //cout << x << "\t" << y << "\t" << angle << endl;

        //Prepare for drawing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        

        opengl.drawRobotOpenGL(robot);

        opengl.drawArrowOpenGL(Point2f(0,80), Point2f(200,300));        

        window.display();
    }

    return MENU;
}