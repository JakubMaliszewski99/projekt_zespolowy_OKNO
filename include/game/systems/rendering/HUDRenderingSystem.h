#pragma once
#include<SFML/Graphics.hpp>
#include "../include/core/core.h"
#include "../include/core/ecs/ECSManager.h"
#include "core/engine/BSP.h"

const int HEALTH_BAR_W = 100;
const int HEALTH_BAR_H = 10;
const int HEALTH_BAR_X = 10;
const int HEALTH_BAR_Y = 10;


class HUDRenderingSystem : public System{
    public:

    HUDRenderingSystem(){
        
        if (!fontLeft.loadFromFile("../data/font/amazdoom/AmazDooMLeft.ttf")) {
            std::cout << "Font didn't load correctly..." << std::endl;
        }
        if (!fontRight.loadFromFile("../data/font/amazdoom/AmazDooMRight.ttf")) {
            std::cout << "Font didn't load correctly..." << std::endl;
        }
        healthBar.setSize(sf::Vector2f(HEALTH_BAR_W, HEALTH_BAR_H));
        healthBar.setPosition(HEALTH_BAR_X, HEALTH_BAR_Y);
        healthBar.setFillColor(sf::Color::Green);

        healthText.setFont(fontLeft);
        healthText.setString("Health");
        healthText.setCharacterSize(24);
        healthText.setFillColor(sf::Color::Red);
        healthText.setPosition(HEALTH_BAR_X + 20, HEALTH_BAR_Y + 20);
    
    }

    void init(std::shared_ptr<ECSManager> manager, 
            std::shared_ptr<sf::RenderWindow> renderWindow,
            Entity playerEntity,
            DebugSettings debugSettings){
                m_manager = manager;
                m_renderWindow = renderWindow;
                m_playerEntity = playerEntity;
            }

    void update(){
        m_renderWindow->draw(healthBar);
        m_renderWindow->draw(healthText);

        m_renderWindow->display();
    }


    private:
    std::shared_ptr<sf::RenderWindow> m_renderWindow;
    std::shared_ptr<ECSManager> m_manager;
    Entity m_playerEntity;
    DebugSettings m_debugSettings;
    sf::RectangleShape healthBar;
    sf::Text healthText;
    sf::Font fontLeft;
    sf::Font fontRight;
};
