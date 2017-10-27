#ifndef ROBOT_H_
#define ROBOT_H_

#include "Commons.h"
#include "Ball.h"

using namespace rodetas;

class Robot {

public:
    Robot();
    Robot(int);

    Robot(int _id, int _x, int _y);

    Robot& operator=(const Robot& r) {
        
        if (this != &r){
            id = r.id;
            radius = r.radius;
            position = r.position;
            target = r.target;
            velocity = r.velocity;
            angle = r.angle;
            command = r.command;
            lastCommand = r.lastCommand;
            lastPositions = (r.lastPositions);
        }

        return *this;
    }

    int y() const;
    int x() const;

    float calculateSpeed();
    void initialize();

    /**
     * Returns if a rodetas::object is near the board
     * \param the object you want to know if is near the board
     * \return true if is near
     */
    bool isBoard();
    
    bool isNull();

    float getVelocity();

    void setRobotId(int);
    int getRobotId();

    void setPotencyFactor(float);
    float getPotencyFactor();

    void setCurveFactpr(float);
    float getCurveFactor();

    void setPosition(Point);
    Point getPosition() const;

    void setAngle(float);
    float getAngle() const;

    void setRadius(float);
    float getRadius();

    void setCommand(Command);
    Command getCommand();

    Command getLastCommand();

    void setTarget(Point);
    Point getTarget() const;

    void setLastPositions(vector<Point>);
    vector<Point>::iterator getLastPositionsBegin();
    vector<Point>::iterator getLastPositionsEnd();

    float cosFrom(Robot) const;
    float cosFrom(Object) const;
    float cosFrom(Point) const;

    float sinFrom(Robot) const;
    float sinFrom(Object) const;
    float sinFrom(Point) const;

    float distanceFrom(Robot) const;
    float distanceFrom(Robot*) const;
    float distanceFrom(Ball) const;
    float distanceFrom(Ball*) const;
    float distanceFrom(Object) const;
    float distanceFrom(Point) const;

private:

    const int velocityPrecision = 10;

    int id;

    float velocity;
    float angle;
    float radius;

    float potencyFactor;
    float curveFactor;

    Point position;
    Point target;

    Command command;
    Command lastCommand;

    vector<Point> lastPositions;

};

#endif