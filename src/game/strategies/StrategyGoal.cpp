#include "StrategyGoal.h"

StrategyGoal::StrategyGoal() : StrategyBase() {

}

void StrategyGoal::apply(Robot* robot){
    if(!robot->isNull()){

        setRobot(robot);
		robot->setCurveFactor(robot->getCurveFactor()*0.5);
		robot->setPotencyFactor(robot->getPotencyFactor()*0.8);

        // define target
        Point target = defineTarget(robot);
        robot->setTarget(target);
        
        // define pwm
        Command movimentationCommand = movimentation.movePlayers(robot);

        // define strategy
        Command strategyCommand = strategy(robot, movimentationCommand);
        
        Command finalPwm = movimentation.progressiveAcell(robot, strategyCommand);
        
        robot->setCommand(finalPwm);
    }
} 

Command StrategyGoal::strategy(Robot* robot, Command command){
	Command c = command;

	c = stopStrategy(c);

	return c;
}

Point StrategyGoal::defineTarget(Robot* robot){

    Point goalTarget = Point(0,0);
	Point ballProjection = data->getBall()->getBallProjection();

	int halfGoal1 = rodetas::imageSize.y/2 + (rodetas::goalSize.y)*0.7;
	int halfGoal2 = rodetas::imageSize.y/2 - (rodetas::goalSize.y)*0.7;

	goalTarget.x = rodetas::imageSize.x*0.10;
	//goalTarget.y = rodetas::imageSize.y/2 - (rodetas::imageSize.y/2-ballProjection.y)/2;
	goalTarget.y = data->getBall()->getBallProjection().y;

	if(goalTarget.y > imageSize.y/2+goalSize.y/3){
		goalTarget.y = imageSize.y/2+goalSize.y/3;
	} else if(goalTarget.y < imageSize.y/2-goalSize.y/3){
		goalTarget.y = imageSize.y/2-goalSize.y/3;
	}

	// manda ir na bola quando ela está dentro da area
	if(((ballProjection.y < halfGoal1 && ballProjection.y > halfGoal2 && ballProjection.x < imageSize.x*0.20))){
		goalTarget = ballProjection;
	}

	// quando esta agarrado manda ir para o centro do gol na tentativa de soltar
	if(robot->isStoppedFor(90) && robot->distanceFrom(goalTarget) > robot->getRadius()*1.5){
		goalTarget.x = imageSize.x*0.1;
		goalTarget.y = imageSize.y*0.5;
    }

	return goalTarget;
}

Command StrategyGoal::stopStrategy(Command command){
    // Para o robo quando atinge o target, alem disso, rotaciona de forma que esteja sempre virado para a bola

    Command c = command;
   
	float distanceTarget = robot->distanceFrom(robot->getTarget());

	if(distanceTarget < robot->getRadius()){

        if ((robot->getAngle() > 80 && robot->getAngle() < 120) || (robot->getAngle() > 260 && robot->getAngle() < 300)) {
            c = movimentation.stop();
        } else {
			if (robot->sinFrom(Point(robot->getTarget().x, 0)) > 0) {
				c = movimentation.turn(robot, Point(robot->getTarget().x, 0), RIGHT_MOVE);
			} else {
				c = movimentation.turn(robot, Point(robot->getTarget().x, 0), LEFT_MOVE);				
			}

			c.pwm1 *= 0.5;
			c.pwm2 *= 0.5;
        }
    }

    return c;
}