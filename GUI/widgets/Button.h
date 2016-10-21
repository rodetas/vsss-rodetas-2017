#ifndef BUTTON_H_
#define BUTTON_H_

#include "../../Header.h"

#include <SFML/Graphics.hpp>

using namespace std;

class Button : public sf::Drawable, public sf::Transformable {
    
public:

    void texturePressed(string m_local){
        m_texturePressed.loadFromFile(m_local);
    }

    void textureNonPressed(string m_local){
        m_textureNonPressed.loadFromFile(m_local);
        m_rectShape.setTexture(&m_textureNonPressed);
    }

    void textureHover(string m_local){
        m_textureHover.loadFromFile(m_local);
    }

    void rectangleSize(int x, int y){
        m_rectShape.setSize(sf::Vector2f(x,y));      
        this->setOrigin(x/2, y/2);
    }

    void textButton(string text, int textSize){
        // if colocado pois quando se chama a funcao mais de uma vez, o texto some
        if(!m_defined){
            m_font.loadFromFile(fontTextPath);

            m_textButton.setFont(m_font);
            m_textButton.setString(text);
            m_textButton.setCharacterSize(textSize);

            m_defined = true;
        }
            
        sf::FloatRect m_textRect = m_textButton.getLocalBounds();
        m_textButton.setOrigin(m_textRect.left , m_textRect.top);        
        m_textButton.setPosition( int( (m_rectShape.getSize().x * 0.25)), 
                                  int( (m_rectShape.getSize().y - m_textRect.height) /2));
    }

    bool buttonEvent(sf::Event event){
        m_pressed = false;

        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved){

            sf::Vector2i mouse;
                mouse.x = (event.type == sf::Event::MouseMoved) ? event.mouseMove.x : event.mouseButton.x;
                mouse.y = (event.type == sf::Event::MouseMoved) ? event.mouseMove.y : event.mouseButton.y;
                
            sf::IntRect buttonArea( this->getPosition().x-m_rectShape.getSize().x/2, this->getPosition().y-m_rectShape.getSize().y/2, m_rectShape.getSize().x, m_rectShape.getSize().y );

            if(buttonArea.contains(mouse)){
                if(event.type == sf::Event::MouseButtonPressed){     
                    m_rectShape.setTexture(&m_texturePressed);
                    m_pressed = true;
                } else if (event.type == sf::Event::MouseButtonReleased){
                    m_rectShape.setTexture(&m_textureNonPressed);
                    m_pressed = false;
                } else {
                    m_rectShape.setTexture(&m_textureHover);
                    m_pressed = false;
                }

            } else {
                m_rectShape.setTexture(&m_textureNonPressed);
                m_pressed = false;
            }
        }

        return m_pressed;
    }

    Button(){
        this->texturePressed(textureButtonPressed);
        this->textureNonPressed(textureButtonNonPressed);
        this->textureHover(textureButtonHover);
        rectangleSize(120,50);
    }

    Button(int posX, int posY, string text){
        this->texturePressed(textureButtonPressed);
        this->textureNonPressed(textureButtonNonPressed);
        this->textureHover(textureButtonHover);

        setPosition(posX, posY);
        textButton(text, 20);
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_rectShape, states);
        target.draw(m_textButton, states);
    }

    sf::RectangleShape m_rectShape;

    sf::Texture m_texturePressed;
    sf::Texture m_textureNonPressed;
    sf::Texture m_textureHover;

    sf::Text m_textButton;
    sf::Font m_font;

    bool m_pressed = false;
    bool m_defined = false;
};
#endif
