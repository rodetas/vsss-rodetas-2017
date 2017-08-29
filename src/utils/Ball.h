#ifndef BALL_H_
#define BALL_H_

#include "Commons.h"

using namespace rodetas;

class Ball {

public:
    
    Ball(){

    }

    Ball(int x, int y){
        position = Point(x,y);
    }

    int x(){
        return position.x;
    }

    int y(){
        return position.y;
    }

    void setPosition(Point _pos){
        position = _pos;
    }

    Point getPosition(){
        return position;
    }

    Point& calculateBallProjection(){
        
        if(lastBallPositions.size() < 9){
            lastBallPositions.push_back(position);
        } else {
            lastBallPositions.pop_back();
            lastBallPositions.insert(lastBallPositions.begin(), position);
            ballProjection.x = position.x + (lastBallPositions[0].x - lastBallPositions[8].x);
            ballProjection.y = position.y + (lastBallPositions[0].y - lastBallPositions[8].y);
    
           /*  if(ballProjection.x > imageSize.x || ballProjection.x < 0 || ballProjection.y > imageSize.y || ballProjection.y < 0){
                ballProjection = lastBallProjection;
            }*/
                
            lastBallProjection = ballProjection;
        }
    
        return ballProjection;
    }

    Point getBallProjection(){
        return ballProjection;
    }

    float distanceFrom(){
        
    }

private:

    Point position;

    Point ballProjection;
    Point lastBallProjection;

    vector<Point> lastBallPositions;

};

#endif