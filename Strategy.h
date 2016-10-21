#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "Header.h"
#include "utils/Structs.h"
#include "Movimentation.h"

using namespace rod;

enum {PARADO, DEFESA, ATAQUE};

class Strategy {
private:

    Movimentation movimentation;

    vector<Object> objects;
    vector<Information> information;
    vector<Point> targets;
    vector<Object> lastBallPositions;
    vector<Command> movements;
    
    Object robot;
    Object ball;

    Object ballProjection;
    Object lastBallProjection;

    float distance_robot_ball;
    float distance_robot_destination;
    float angle_robot_destination;
    float distance_ball_destination;
    float sinAngle_robot_destination;
    float cosAngle_robot_destination;
    float cos_robot_ball;
    float convertRadiansToDegrees;

    float curve;

    Point imageSize;
    Point goalSize;
    Point2f goalArea;

    Point attackDestination;
    Point defenseDestination;
    Point goalDestination;

    int frames;

    int attackNumber;
    int defenseNumber;
    int goalNumber;

    int state;

    Clock clockChange;

public:
    Strategy();

    void initialize(Point, Point);
    void handleStrategies();
    void defineFunctions();

    void calcBallProjection();
    void updateCalculus(Object, Point);    
    void changeCoordinates();

    void applyAttackStrategy();
    void applyDefenseStrategy();
    void applyGoalStrategy();

    bool isBoard(Object);
    void cornerStrategy(string);
    
    Point applyPotencialField(Point, Object, Object);

    Point setAttackTarget();
    Point setDefenseTarget();
    Point setGoalTarget();

    void setObjects(vector<Object> o);
    void setPower(float a);
    void setPowerCurve(float a);
    void setVecTarget(int, Point);
    
    vector<string> getMovements();
    vector<Point> getTargets();
    vector<Information> getInformation();
};
#endif