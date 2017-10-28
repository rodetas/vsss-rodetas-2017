#include "StrategyGoal.h"

StrategyGoal::StrategyGoal() : StrategyBase() {

}

Command StrategyGoal::strategy(Robot* robot, Command command){
	Command c = command;

	c = stopStrategy(c);

	return c;
}

Point StrategyGoal::defineTarget(Robot* robot){

    Point goalTarget = Point(0,0);
	Point ballProjection = data->getBall()->getBallProjection();

	goalTarget.x = rodetas::imageSize.x*0.15;
	goalTarget.y = rodetas::imageSize.y/2 - (rodetas::imageSize.y/2-ballProjection.y)/2;

	return goalTarget;
}

Command StrategyGoal::stopStrategy(Command command){
    // Para o robo quando atinge o target, alem disso, rotaciona de forma que esteja sempre virado para a bola

    Command c = command;
    float maxDistance = robot->getRadius()*3;
	float distanceTarget = robot->distanceFrom(robot->getTarget());
	
	if(robot->getVelocity() > rodetas::imageSize.x*0.05){
		maxDistance = robot->getRadius()*6;
	}

	if(distanceTarget < maxDistance){
		c.pwm1 = command.pwm1*(distanceTarget/maxDistance);
		c.pwm2 = command.pwm2*(distanceTarget/maxDistance);
	}

	if(distanceTarget < robot->getRadius()){

        if ((robot->getAngle() > 80 && robot->getAngle() < 120) || (robot->getAngle() > 260 && robot->getAngle() < 300)) {
            c = movimentation.stop();
        } else {
			c = movimentation.turnRight(100, 100);
        }
    }

    return c;
}