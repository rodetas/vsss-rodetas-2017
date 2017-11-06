#include "StrategyDefense.h"

StrategyDefense::StrategyDefense() : StrategyBase()	{

}

Command StrategyDefense::strategy(Robot* robot, Command command){
	Command c = command;

	c = stopStrategy(c);
	//c = collisionStrategy(c);
	//c = blockedStrategy(c);

	return c;
}

Point StrategyDefense::defineTarget(Robot* robot){
	Point target;
	Point ballProjection = data->getBall()->getBallProjection();
	Ball* ball = data->getBall();

	// altera o ponto de destino dependendo do sentido da bola, evitando bater no outro robo
	if(robot->x() > imageSize.x*0.6){
		if(robot->y() > imageSize.y/2){
			if(ballProjection.y < ball->y()){
				target = Point(imageSize.x*0.5, imageSize.y*0.8);
			} else {
				target = Point(imageSize.x*0.5, imageSize.y*0.2);
			}
		} else {
			if(ballProjection.y < ball->y()){
				target = Point(imageSize.x*0.5, imageSize.y*0.8);
			} else {
				target = Point(imageSize.x*0.5, imageSize.y*0.2);
			}
		}	
		
	}else{
		// se a bola esta no ataque posiciona o robo no meio do campo
		target = Point(imageSize.x/2, imageSize.y/2);
//		target = Point(imageSize.x/2, ball->y());		// trocar para seguir o Y da bola
	}

	// posiciona o robo na defesa para facilitar a troca de posicao com o goleiro
	if(ballProjection.x < imageSize.x/2){
		if(ballProjection.y > imageSize.y/2){
			target = Point(imageSize.x*0.3, imageSize.y*0.2);
		} else {
			target = Point(imageSize.x*0.3, imageSize.y*0.8);
		}
	}

	return target;
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