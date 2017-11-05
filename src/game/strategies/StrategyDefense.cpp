#include "StrategyDefense.h"

StrategyDefense::StrategyDefense() : StrategyBase()	{

}

Command StrategyDefense::strategy(Robot* robot, Command command){
	Command c = command;

	c = stopStrategy(c);
	c = collisionStrategy(c);

	return c;
}

Point StrategyDefense::defineTarget(Robot* robot){

	Point defenseTarget = Point(imageSize.x/2, data->getBall()->getBallProjection().y);

	return defenseTarget;
}

Command StrategyDefense::collisionStrategy(Command _command){  
	Command c = _command;

	int angle_ball = calcAngle(robot->getPosition(), data->getBall()->getPosition());
	int angle_defense = calcAngle(data->getRobot("attack").getPosition(), data->getBall()->getPosition());

	angle_ball = (angle_ball + 360) % 360; // converter para 0 - 360
	angle_defense = (angle_defense + 360) % 360;

	float sin_angle = sin(angle_ball - angle_defense);

	if ( distance(robot->getProjection(), data->getRobot("attack").getProjection() ) < robot->getRadius() * 8){

		char robotWay = c.direcao;
		char attackWay = data->getRobot("attack").getCommand().direcao;
		if(robotWay == attackWay){
			// os dois de frente ou de costas
			c.pwm2 = c.pwm2 * abs(sin_angle) + 0.3;

		} else if(robotWay != attackWay){
			// os robos estao em sentidos opostos
			c.pwm2 = c.pwm2 * abs(sin_angle) + 0.3;
		}

	}

	return c;
}