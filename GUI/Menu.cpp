#include "Menu.h"

Menu::Menu(){
	button.resize(5);
	drawComponents();
}

Menu::~Menu(){

}

int Menu::menu(){
	
	sf::RenderWindow window(sf::VideoMode(800, 600), "Rodetas Menu");

	while(window.isOpen()){

		window.clear();

		window.draw(background);

		for (int i = 0; i < button.size(); i++){
			window.draw(button[i]);
		}

		sf::Event event;
	    while (window.pollEvent(event)){

	        if (event.type == sf::Event::Closed){
	            window.close();
	            exit(0);

	        } else if(event.type == sf::Event::KeyPressed){
	        	if(event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1){
	        		window.close();
	        		return GAME;
	        	} else if(event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Numpad2){
	        		window.close();
	        		return CALIBRATION;
	        	} else if(event.key.code == sf::Keyboard::Num3 || event.key.code == sf::Keyboard::Numpad3){
	        		window.close();
	        		return SIMULATOR;
	        	} else if(event.key.code == sf::Keyboard::T){
	        		window.close();
	        		return TEST; 
	        	} else if(event.key.code == sf::Keyboard::Num4){
	        		return ARDUINO;

	        	} else if(event.key.code == sf::Keyboard::Escape){
	        		window.close();
	        		exit(0);
	        	}

	        } else if (button[0].buttonEvent(event)){
	        	window.close();
	        	return GAME;
	        } else if (button[1].buttonEvent(event)){
	        	window.close();
	        	return CALIBRATION;
	        } else if (button[2].buttonEvent(event)){
	        	window.close();
	        	return SIMULATOR;
	        } else if (button[3].buttonEvent(event)){
	        	window.close();
	        	return ARDUINO;
	        } else if (button[4].buttonEvent(event)){
	        	window.close();
	        	exit(0);
	        }
	    }

	    window.display();
	}

}

void Menu::drawComponents(){

	for (int i = 0; i < button.size(); i++){
		button[i].rectangleSize(270,40);
	}

	button[0].setPosition(400,200);
	button[0].textButton("1-Jogar", 20);

	button[1].setPosition(400,250);
	button[1].textButton("2-Calibrar", 20);

	button[2].setPosition(400,300);
	button[2].textButton("3-Simular Posicoes", 20);

	button[3].setPosition(400,350);
	button[3].textButton("4-Upload Arduino", 20);

	button[4].setPosition(400,400);
	button[4].textButton("5-Sair", 20);

	texture.loadFromFile(backgroundPath_menu);
	background.setSize(sf::Vector2f(800,600));
	background.setTexture(&texture);
}