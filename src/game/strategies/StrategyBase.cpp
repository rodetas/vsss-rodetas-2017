#include "StrategyBase.h"

int StrategyBase::nStrategies = 0;

StrategyBase::StrategyBase(){
    robotState = PARADO;
	nStrategies++;

    data = Strategy::getInstance();
}

void StrategyBase::apply(Robot* robot){
    if(!robot->isNull()){

        setRobot(robot);

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

void StrategyBase::move(Robot* robot){
    setRobot(robot);
    // define pwm
    Command movimentationCommand = movimentation.movePlayers(robot);

    // define strategy
    Command strategyCommand = strategy(robot, movimentationCommand);

    strategyCommand = stopStrategy(strategyCommand);

    Command finalPwm = movimentation.progressiveAcell(robot, strategyCommand);

    robot->setCommand(finalPwm);
}

 Command StrategyBase::cornerStrategy(Command command){
    // movement along the corners
    Command c = command;

	if (robot->isBoard() && robot->isStopped()){
        
        // girar caso robo esteja preso de frente pra parede
        if (robot->cosFrom(data->getBall()->getPosition()) > -0.9 && robot->cosFrom(data->getBall()->getPosition()) < 0.9) {
            if (robot->sinFrom(data->getBall()->getPosition()) > 0) {
                c = movimentation.turn(robot, data->getBall()->getPosition(), RIGHT_MOVE);
            } else {
                c = movimentation.turn(robot, data->getBall()->getPosition(), LEFT_MOVE);
            }
        } 
        
        // girar caso robo prenda a bola na parede
        if (robot->distanceFrom(data->getBall()->getPosition()) < robot->getRadius()*1.5) {

            if (robot->y() > (rodetas::imageSize.y/2)){
                c = movimentation.turnLeft(255, 255);	
            } else {
                c = movimentation.turnRight(255, 255);
            }
        }
    }

    return c;
}

Command StrategyBase::collisionStrategy(Command _command){   
}

Command StrategyBase::kickStrategy(Command _command){
}

Command StrategyBase::stopStrategy(Command _command){
    // Para o robo quando atinge o target, alem disso, rotaciona de forma que esteja sempre virado para a bola

    Command c = _command;
    float maxDistance = robot->getRadius()*3;
	float distanceTarget = robot->distanceFrom(robot->getTarget());
	
	if(robot->getVelocity() > rodetas::imageSize.x*0.05){
		maxDistance = robot->getRadius()*6;
	}

	if(distanceTarget < maxDistance){
		c.pwm1 = c.pwm1*(distanceTarget/maxDistance);
		c.pwm2 = c.pwm2*(distanceTarget/maxDistance);
	}

	if(distanceTarget < robot->getRadius()){

        if (robot->cosFrom(data->getBall()->getBallProjection()) < -0.8 || robot->cosFrom(data->getBall()->getBallProjection()) > 0.8) {
            c = movimentation.stop();
 
        } else {

            if (robot->sinFrom(data->getBall()->getPosition()) > 0) {
                //c = movimentation.turnRight(pwm, pwm);
                c = movimentation.turn(robot, data->getBall()->getBallProjection(), RIGHT_MOVE);
            } else {
                //c = movimentation.turnLeft(pwm, pwm);
                c = movimentation.turn(robot, data->getBall()->getBallProjection(), LEFT_MOVE);
            }

            c.pwm1 *= 0.8;
            c.pwm2 *= 0.8;
        }
    }

    return c;
}

Command StrategyBase::blockedStrategy(Command _command){
    Command c = _command;
    
    if(robot->isStoppedFor(90) && robot->distanceFrom(robot->getTarget()) > robot->getRadius()*6){
        if (c.direcao == FORWARD_MOVE) {
            c = Command(255,255,BACK_MOVE);
        } else if(c.direcao == BACK_MOVE){
            c = Command(255,255,FORWARD_MOVE);
        }
    }

    return c;
}

// FICA CONFUSO QUANDO ESTA EXTAMENTE NO MEIO
Point StrategyBase::applyPotencialField(Point _target, Point repulsionPoint) {
    Point t = _target;
    float s = sin((calcAngle(robot->getPosition(), repulsionPoint))/RADIAN_TO_DEGREE);
    

    if (distance(robot->getProjection(), repulsionPoint) < robot->getRadius() * 6 && 
        s < 0.3 && s > -0.3) {
        t.x = repulsionPoint.x ;
        t.y = repulsionPoint.y + robot->getRadius() * 6;
    }

    return t;
} 

void StrategyBase::setRobot(Robot* _robot){
    robot = _robot;
}

Robot* StrategyBase::getRobot(){
    return robot;
}

int StrategyBase::getNumStrategies(){
	return nStrategies;
}