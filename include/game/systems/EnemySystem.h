#pragma once
#include "../../core/ecs/System.h"
#include "../../core/math/collisions.h"
#include "../include/core/ecs/ECSManager.h"
#include "../components/EnemyComponent.h"
#include "../components/TransformComponent.h"

const float DEFAULT_ENEMY_ROTATION_SPEED = 2.4f;
const float PATROL_ROTATION_LIMIT = 180.0f * (M_PI / 180.0f);
const float PATROL_PAUSE_DURATION = 3.0f;
const float ENEMY_SPEED = 50.0f;


class EnemySystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager, Entity playerEntity) { 
    m_manager = manager; 
    m_playerEntity = playerEntity;
    }

  void update(float dt, std::vector<Entity> enemyEntities){
    auto &playerTransform = m_manager->getComponent<TransformComponent>(m_playerEntity);

    for (auto const &entity : enemyEntities){
      auto &transform = m_manager->getComponent<TransformComponent>(entity);
      auto &enemy = m_manager->getComponent<EnemyComponent>(entity);
      auto &health = m_manager->getComponent<HealthComponent>(entity);

      // handle logic differently depending on the game state
      if(enemy.isAlive){
        switch(enemy.state){
          case Idle:
            //"Do nothing" state
            break;
          case Patrol: {
            //Looking around and rotating after some time (default state)
            if(enemy.rotationProgress < PATROL_ROTATION_LIMIT){
              float rotationThisFrame = DEFAULT_ENEMY_ROTATION_SPEED * dt;
              enemy.rotationProgress += rotationThisFrame;
              transform.angle -= rotationThisFrame;

              if(enemy.rotationProgress >= PATROL_ROTATION_LIMIT)
                enemy.pauseRotationTimer = 0.0f;

            }else if(enemy.pauseRotationTimer < PATROL_PAUSE_DURATION){
              enemy.pauseRotationTimer += dt;

              if(enemy.pauseRotationTimer >= PATROL_PAUSE_DURATION){
                enemy.rotationProgress = 0.0f;
              }
            }
            sf::Vector2f playerPosition(playerTransform.positionX, playerTransform.positionY);
            //Change state into Chase if the player comes into FOV
            if(isPlayerInView(transform, playerPosition)){
              enemy.state = Chase;
            }

            break;
          }
          case Chase:
          {
            //TODO Change actual player position to last known player position when the player comes out of the field of view
            //Follow the player 
            // 1. Get the player's position
            sf::Vector2f playerPosition(playerTransform.positionX, playerTransform.positionY);
            
            // 2. Calculate the direction vector to the player
            sf::Vector2f directionToPlayer = playerPosition - sf::Vector2f(transform.positionX, transform.positionY);

            // 3. Normalize the direction vector
            float length = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);
            if (length != 0) {
                directionToPlayer /= length;  // Normalize the vector
            }

            // 4. Move the enemy towards the player
            transform.positionX += directionToPlayer.x * ENEMY_SPEED * dt;
            transform.positionY += directionToPlayer.y * ENEMY_SPEED * dt;

            // 5. Update the enemy's rotation to face the player
            transform.angle = std::atan2(directionToPlayer.y, directionToPlayer.x);

            if(!isPlayerInView(transform, playerPosition)){
              enemy.state = Patrol;
            }
            
            break;
          }
          case Attack:
            //Shoot the player
            break;
          default:
            break;
        }
      }
    }

  }

  bool isEnemy(int type) {
    switch (type) {
    case eImp:
    case eShotgunGuy:
    case eZombieMan:
      return true;
    default:
      return false;
    }
  }

  bool isPlayerInView(const TransformComponent &transform, const sf::Vector2f &playerPosition){
    sf::Vector2f enemyPosition(transform.positionX, transform.positionY);
    sf::Vector2f directionToPlayer = playerPosition - enemyPosition;

    // Calculate the distance between the enemy and the player
    float distanceToPlayer = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);

    // Check if the player is within the specified distance
    if (distanceToPlayer > 700.0f) {
        return false;  // The player is too far away
    }

    float angleToPlayer = std::atan2(directionToPlayer.y, directionToPlayer.x);
    float enemyAngle = transform.angle;

    // Calculate the difference between the enemy's forward direction and the direction to the player
    float fov = FOV_ENEMIES * (M_PI / 180.0f);
    float halfFov = fov / 2.0f;

    float angleDifference = std::atan2(std::sin(angleToPlayer - enemyAngle), std::cos(angleToPlayer - enemyAngle));
    return std::abs(angleDifference) <= halfFov;
  }

private:
  std::shared_ptr<ECSManager> m_manager;
  Entity m_playerEntity;
  
};