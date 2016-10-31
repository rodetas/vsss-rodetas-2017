#include "OpenGL.h"

OpenGL::OpenGL(){

}

void OpenGL::initializeOpenGL(Point2f windowSize, Point2f viewSize, float  sizeRobot){

    window = windowSize;
    size = sizeRobot;
    view = viewSize;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);

    // Disable lighting
    glDisable(GL_LIGHTING);

    // Configure the viewport (the same size as the window)
    glViewport((window.x/2 - view.x/2), (window.y/2 - view.y/2), view.x, view.y);

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    GLfloat ratio = static_cast<float>(window.x) / window.y;
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 100.f);

}

Point2f OpenGL::openGLCoordinates(Point2f openCVCoordinates){ 
    // opengl axis with frustum 1, variation -100 to 100
    openCVCoordinates.x =  openCVCoordinates.x - (window.x/2 - view.x/2);
    openCVCoordinates.y = -openCVCoordinates.y + (window.y/2 - view.y/2);
    float ratio = (view.x / view.y) * 100;

    return Point2f( (openCVCoordinates.x * ratio) / (view.x/2) - ratio, 
                    (openCVCoordinates.y * 100)   / (view.y/2) + 100);
}

void OpenGL::drawRobotOpenGL(Robot_Draw robot){

    Point2f pointOpenGL = openGLCoordinates(Point2f(robot.x, robot.y));

    //Prepare for drawing
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();
    glTranslatef(pointOpenGL.x, pointOpenGL.y, -100);
    glRotatef(robot.angle, 0.f, 0.f, 1.f);

    float sizeCube = size; //x2
    float sizeColor = size*0.8;

    // Face frontal
    glBegin(GL_QUADS);
        glColor3ub(robot.color_team.r, robot.color_team.g, robot.color_team.b);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f( sizeCube,  sizeCube, sizeCube);
        glVertex3f(-sizeCube,  sizeCube, sizeCube);
        glVertex3f(-sizeCube, -sizeCube, sizeCube);
        glVertex3f( sizeCube, -sizeCube, sizeCube);
    
        glColor3ub(robot.color_player.r, robot.color_player.g, robot.color_player.b);
        glVertex3f(sizeCube - sizeColor,  sizeCube,  sizeCube);
        glVertex3f(sizeCube - sizeColor,  sizeCube - sizeColor, sizeCube);
        glVertex3f(sizeCube, sizeCube - sizeColor,  sizeCube);
        glVertex3f(sizeCube, sizeCube,  sizeCube);
    glEnd();

    // Face posterior
    glBegin(GL_QUADS);
        glColor3f(0.f, 0.f, 0.f);
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f( sizeCube,  sizeCube, -sizeCube);
        glVertex3f( sizeCube, -sizeCube, -sizeCube);
        glVertex3f(-sizeCube, -sizeCube, -sizeCube);
        glVertex3f(-sizeCube,  sizeCube, -sizeCube);
    glEnd();

    // Face lateral esquerda
    glBegin(GL_QUADS);
        glColor3f(0.f, 0.f, 0.f);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(-sizeCube,  sizeCube,  sizeCube);
        glVertex3f(-sizeCube,  sizeCube, -sizeCube);
        glVertex3f(-sizeCube, -sizeCube, -sizeCube);
        glVertex3f(-sizeCube, -sizeCube,  sizeCube);
    glEnd();

    // Face lateral direita
    glBegin(GL_QUADS);
        glColor3f(0.f, 0.f, 0.f);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f(sizeCube,  sizeCube,  sizeCube);
        glVertex3f(sizeCube, -sizeCube,  sizeCube);
        glVertex3f(sizeCube, -sizeCube, -sizeCube);
        glVertex3f(sizeCube,  sizeCube, -sizeCube);
    glEnd();

    // Face superior
    glBegin(GL_QUADS);
        glColor3f(0.f, 0.f, 0.f);
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(-sizeCube, sizeCube, -sizeCube);
        glVertex3f(-sizeCube, sizeCube,  sizeCube);
        glVertex3f( sizeCube, sizeCube,  sizeCube);
        glVertex3f( sizeCube, sizeCube, -sizeCube);
    glEnd();

    // Face inferior
    glBegin(GL_QUADS);
        glColor3f(0.f, 0.f, 0.f);
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(-sizeCube, -sizeCube, -sizeCube);
        glVertex3f( sizeCube, -sizeCube, -sizeCube);
        glVertex3f( sizeCube, -sizeCube,  sizeCube);
        glVertex3f(-sizeCube, -sizeCube,  sizeCube);
    glEnd();
}

void OpenGL::drawArrowOpenGL(Point2f initialPoint, Point2f finalPoint){

    Point2f initial = openGLCoordinates(initialPoint);
    Point2f final = openGLCoordinates(finalPoint);
    
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();
    glTranslatef(1.f, 1.f, -100.f);
    glRotatef(0.f, 0.f, 0.f, 0.f);

    glBegin(GL_LINE_STRIP);
        glVertex2d(initial.x, initial.y);
        glVertex2d(final.x,final.y);
    glEnd();
}