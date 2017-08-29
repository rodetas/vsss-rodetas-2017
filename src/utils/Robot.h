#ifndef ROBOT_H_
#define ROBOT_H_

#include "Commons.h"
#include "Ball.h"

using namespace rodetas;

class Robot {

public:
    Robot();
    Robot(int);

    Robot& operator=(const Robot& other)
    {
        // check for self-assignment
        if(&other == this)
            return *this;

        
        return *this;
    }

    int y();
    int x();

    float calculateSpeed();
    void initialize();

    /**
     * Returns if a rodetas::object is near the board
     * \param the object you want to know if is near the board
     * \return true if is near
     */
    bool isBoard();

    float getVelocity();

    void setId(int);
    int getId();

    void setPosition(Point);
    Point getPosition();

    void setCommand(Command);
    Command getCommand();

    void setTarget(Point);
    Point getTarget();

    void setLastPositions(vector<Point>);
    vector<Point>::iterator getLastPositionsBegin();
    vector<Point>::iterator getLastPositionsEnd();

    float cosFrom(Robot);
    float cosFrom(Object);
    float cosFrom(Point);

    float sinFrom(Robot);
    float sinFrom(Object);
    float sinFrom(Point);

    float distanceFrom(Robot);
    float distanceFrom(Ball);
    float distanceFrom(Object);
    float distanceFrom(Point);

private:

    const int velocityPrecision = 60;

    int id;

    float velocity;
    float angle;

    Point position;
    Point target;

    Command command;

    vector<Point> lastPositions;

};

#endif