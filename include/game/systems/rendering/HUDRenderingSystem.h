#pragma once
#include "../include/core/core.h"
#include "../include/core/ecs/ECSManager.h"
#include "core/engine/BSP.h"
#include <SFML/Graphics.hpp>

const float GUN_TEXTURE_X_MOV_FREQ = 5;
const float GUN_TEXTURE_X_MOV_AMP = 80;
const float GUN_TEXTURE_Y_MOV_FREQ = 5;
const float GUN_TEXTURE_Y_MOV_AMP = 50;

class HUDRenderingSystem : public System {
public:
  HUDRenderingSystem() {

    if (!fontLeft.loadFromFile("../data/font/amazdoom/AmazDooMLeft.ttf")) {
      std::cout << "Font didn't load correctly..." << std::endl;
    }
    if (!fontRight.loadFromFile("../data/font/amazdoom/AmazDooMRight.ttf")) {
      std::cout << "Font didn't load correctly..." << std::endl;
    }
    if (!hudTexture.loadFromFile("../data/textures/HUD.png")) {
      std::cout << "Could not load HUD texture!" << std::endl;
    }
    if (!handGunTexture.loadFromFile("../data/textures/Gun.png")) {
      std::cout << "Could not load HandGun texture!" << std::endl;
    }

    healthBar.setFillColor(sf::Color::Green);

    missingHealthBar.setFillColor(sf::Color::Red);

    healthText.setFont(fontLeft);
    healthText.setString("Health");
    healthText.setCharacterSize(30);
    healthText.setFillColor(sf::Color::Red);

    ammoText.setFont(fontRight);
    ammoText.setString("Ammo");
    ammoText.setCharacterSize(30);
    ammoText.setFillColor(sf::Color::Red);

    ammoAmountText.setFont(fontRight);
    ammoAmountText.setString("500");
    ammoAmountText.setCharacterSize(60);
    ammoAmountText.setFillColor(sf::Color::Red);

    hudSprite.setTexture(hudTexture);
    handGunSprite.setTexture(handGunTexture);
  }

  void init(std::shared_ptr<ECSManager> manager,
            std::shared_ptr<sf::RenderWindow> renderWindow, Entity playerEntity,
            DebugSettings debugSettings) {
    m_manager = manager;
    m_renderWindow = renderWindow;
    m_playerEntity = playerEntity;
  }

  void scaleHUD() {
    sf::Vector2u windowSize = m_renderWindow->getSize();
    sf::Vector2u hudTextureSize = hudTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / hudTextureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / hudTextureSize.y;

    hudSprite.setScale(scaleX, scaleY);

    healthBarX = windowSize.x * 0.33;
    healthBarY = windowSize.y * 0.88;
    defaultHealthBarWidth = windowSize.x * 0.1;
    healthBarWidth = defaultHealthBarWidth;
    healthBarHight = windowSize.y * 0.015;

    healthBar.setSize(sf::Vector2f(healthBarWidth, healthBarHight));
    healthBar.setPosition(healthBarX, healthBarY);

    missingHealthBar.setSize(sf::Vector2f(defaultHealthBarWidth, healthBarHight));
    missingHealthBar.setPosition(healthBarX, healthBarY);

    defaultHandGunSpriteX = windowSize.x * 0.41;
    defaultHandGunSpriteY = windowSize.y * 0.65;
    handGunSpriteX = defaultHandGunSpriteX;
    handGunSpriteY = defaultHandGunSpriteY;

    handGunSprite.setPosition(handGunSpriteX, handGunSpriteY);



    healthTextX = healthBarX + 50;
    healthTextY = healthBarY + 25;
    healthText.setPosition(healthTextX, healthTextY);

    ammoAmountTextX = windowSize.x * 0.63;
    ammoAmountTextY = windowSize.y * 0.85;
    ammoAmountText.setPosition(ammoAmountTextX, ammoAmountTextY);

    ammoTextX = ammoAmountTextX + 7;
    ammoTextY = ammoAmountTextY + 60;
    ammoText.setPosition(ammoTextX, ammoTextY);
  }

  void update() {
    auto &state = m_manager->getComponent<PlayerStateComponent>(m_playerEntity);
    auto &health = m_manager->getComponent<HealthComponent>(m_playerEntity);

    if(health.health > 0)
      healthBarWidth = defaultHealthBarWidth * health.health / 100;
    else
      healthBarWidth = 0.0f;
      
    healthBar.setSize(sf::Vector2f(healthBarWidth, healthBarHight));

    if (state.isMovingForward || state.isMovingBackwards ||
        state.isMovingRight || state.isMovingLeft) {
      sf::Time elapsed = gunTextureClock.getElapsedTime();
      float time = elapsed.asSeconds();

      handGunSpriteX =
          defaultHandGunSpriteX +
          std::sin(time * GUN_TEXTURE_X_MOV_FREQ) * GUN_TEXTURE_X_MOV_AMP;
      handGunSpriteY = defaultHandGunSpriteY -
                       std::abs(std::sin(time * GUN_TEXTURE_X_MOV_FREQ) *
                                GUN_TEXTURE_X_MOV_AMP);
      handGunSprite.setPosition(handGunSpriteX, handGunSpriteY);
    }

    m_renderWindow->draw(handGunSprite);
    m_renderWindow->draw(hudSprite);
    m_renderWindow->draw(missingHealthBar);
    m_renderWindow->draw(healthBar);
    m_renderWindow->draw(healthText);
    m_renderWindow->draw(ammoAmountText);
    m_renderWindow->draw(ammoText);

    m_renderWindow->display();
  }

private:
  std::shared_ptr<sf::RenderWindow> m_renderWindow;
  std::shared_ptr<ECSManager> m_manager;
  Entity m_playerEntity;
  DebugSettings m_debugSettings;

  sf::RectangleShape healthBar;
  int defaultHealthBarWidth;
  int healthBarWidth;
  int healthBarHight;
  int healthBarX;
  int healthBarY;

  sf::RectangleShape missingHealthBar;

  sf::Text healthText;
  int healthTextX;
  int healthTextY;

  sf::Text ammoText;
  int ammoTextX;
  int ammoTextY;

  sf::Text ammoAmountText;
  int ammoAmountTextX;
  int ammoAmountTextY;

  sf::Font fontLeft;
  sf::Font fontRight;

  sf::Texture hudTexture;
  sf::Sprite hudSprite;

  sf::Texture handGunTexture;
  sf::Sprite handGunSprite;
  int defaultHandGunSpriteY;
  int defaultHandGunSpriteX;
  int handGunSpriteX;
  int handGunSpriteY;

  sf::Clock gunTextureClock;
};
