#ifndef STRATEGY_H_
#define STRATEGY_H_

#include "Header.h"
#include "Movimentation.h"
#include "utils/Structs.h"
#include "CRUD/Manipulation.h"

using namespace rodetas;

enum {PARADO, DEFESA, ATAQUE};

class Strategy {
private:

    Movimentation movimentation;
    Manipulation manipulation;

    vector<rodetas::Object> objects;
    vector<Information> information;
    vector<Point> targets;
    vector<rodetas::Object> lastBallPositions;
    vector<Command> movements;
    
    rodetas::Object robot;
    rodetas::Object ball;

    rodetas::Object ballProjection;
    rodetas::Object lastBallProjection;

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

public:
    Strategy();

    void initialize();
    void handleStrategies();
    void defineFunctions();

    void calcBallProjection();
    void updateCalculus(rodetas::Object, Point);    
    void changeCoordinates();

    void applyAttackStrategy();
    void applyDefenseStrategy();
    void applyGoalStrategy();

    bool isBoard(rodetas::Object);
    void cornerStrategy(string);
    
    Point applyPotencialField(Point, rodetas::Object, rodetas::Object);

    Point setAttackTarget();
    Point setDefenseTarget();
    Point setGoalTarget();

    void setObjects(vector<rodetas::Object> o);
    void setPowerPotency(float a);
    void setPowerCurve(float a);
    void setVecTarget(int, Point);
    
    vector<Command> getMovements();
    vector<Point> getTargets();
    vector<Information> getInformation();
};
#endif