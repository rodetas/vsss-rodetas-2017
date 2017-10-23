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
        
        movimentation.setPotencyFactor(data->getPotencyFactor());
        movimentation.setCurveFactor(data->getCurveFactor());
        
        // define pwm
        Command movimentationCommand = movimentation.movePlayers(robot);

        // define strategy
        Command strategyCommand = strategy(robot, movimentationCommand);

        /* cout << strategyCommand << "\t";
        cout << robot.getLastCommand() << endl; */
        
        Command finalPwm = movimentation.progressiveAcell(robot, strategyCommand);
        
        robot->setCommand(finalPwm);
    }
} 

 Command StrategyBase::cornerStrategy(Command command){
	
    // movement along the corners
   
	if (robot->isBoard()){
        //command = movimentation.stop();
        //cout<<"Canto"<<endl;
        if(robot->isStopped()){
           // cout<<"Parado"<<endl; 
        }
        
        if (robot->distanceFrom(data->getBall()) < 55){	
           // cout<<"Bola presa"<<endl;	

			if (robot->y() > (rodetas::imageSize.y/2)){
                //cout<<"Vira esquerda"<<endl;
				command = movimentation.turnLeft(120, 120);	
		    } else {
                //cout<<"Vira direita"<<endl;
                command = movimentation.turnRight(120, 120);
			}
		}
    }
    return command;
}

Command StrategyBase::stopStrategy(Command command){
    Command c = command;
    float maxDistance = robot->getRadius()*3;
	float distanceTarget = robot->distanceFrom(robot->getTarget());
	
	 // trocar pra imagesize
	if(robot->getVelocity() > rodetas::imageSize.x*0.05){
		maxDistance = robot->getRadius()*6;
	}

	if(distanceTarget < maxDistance){
		c.pwm1 = command.pwm1*(distanceTarget/maxDistance);
		c.pwm2 = command.pwm2*(distanceTarget/maxDistance);
	}

	if(distanceTarget < robot->getRadius()){

        if (robot->cosFrom(data->getBall().getPosition()) < -0.9) {
            c = movimentation.stop();
        } else if (robot->cosFrom(data->getBall().getPosition()) > 0.9){ 
            c = movimentation.stop();
    
        } else {
            if (robot->sinFrom(data->getBall().getPosition()) > 0) {
                c = movimentation.turnRight(100, 100);
            } else {
                c = movimentation.turnLeft(100, 100);
            }
        }
    }

    return c;
}

// FICA CONFUSO QUANDO ESTA EXTAMENTE NO MEIO
Point StrategyBase::applyPotencialField(const Point& target, const Point& toRepulsion, const Point& toDestination) const {
    
    Point2i repulsion;
    Point2i factorRepulsion = Point2i(5000,20000);//{ 5000, 20000 };

    float sin_repulsion_destination = sin((calcAngle(toDestination, toRepulsion))/RADIAN_TO_DEGREE);
    float cos_repulsion_destination = cos((calcAngle(toDestination, toRepulsion))/RADIAN_TO_DEGREE);
    float distance_repulsion_destination = distance(toRepulsion, toDestination);

    if(sin_repulsion_destination > 0){
        sin_repulsion_destination = 1 - sin_repulsion_destination;
    } else {
        sin_repulsion_destination = abs(sin_repulsion_destination) - 1;
    }

    repulsion.x = (cos_repulsion_destination / (distance_repulsion_destination * 0.6)) * factorRepulsion.x;
    repulsion.y = (sin_repulsion_destination / (distance_repulsion_destination * 0.6)) * factorRepulsion.y;

    return Point(repulsion.x, repulsion.y);
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