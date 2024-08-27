#pragma once
#include "../../core/ecs/System.h"
#include "../../core/math/collisions.h"
#include "../include/core/ecs/ECSManager.h"
#include "../components/EnemyComponent.h"
#include "../components/TransformComponent.h"

const float DEFAULT_ENEMY_ROTATION_SPEED = 1.2f;
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
    auto &playerHealth = m_manager->getComponent<HealthComponent>(m_playerEntity);
    auto &playerState = m_manager->getComponent<PlayerStateComponent>(m_playerEntity);

    for (auto const &entity : enemyEntities){
      auto &transform = m_manager->getComponent<TransformComponent>(entity);
      auto &enemy = m_manager->getComponent<EnemyComponent>(entity);
      auto &health = m_manager->getComponent<HealthComponent>(entity);

      if(health.health <= 0){
        enemy.isAlive = false;
      }
      std::cout << health.health << std::endl;

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

            //6. Increment the chasing timer
            enemy.chasingTimer += dt;

            if(!isPlayerInView(transform, playerPosition)){
              enemy.state = Patrol;
            }else{
              if(length < 500.0f && enemy.chasingTimer >= 2.0f){
                enemy.state = Attack;
                enemy.chasingTimer = 0.0f;
              }
            }
            
            break;
          }
          case Attack:
            
            enemy.attackingTimer += dt;
            
            //wait 2 seconds
            if(enemy.attackingTimer >= 1.0f){
              //fire
              // Check if the player is in the line of fire
            sf::Vector2f enemyPosition(transform.positionX, transform.positionY);
            sf::Vector2f playerPosition(playerTransform.positionX, playerTransform.positionY);

            // Direction the enemy is facing
            sf::Vector2f enemyDirection = sf::Vector2f(std::cos(transform.angle), std::sin(transform.angle));

            // Direction from the enemy to the player
            sf::Vector2f directionToPlayer = playerPosition - enemyPosition;
            float length = std::sqrt(directionToPlayer.x * directionToPlayer.x + directionToPlayer.y * directionToPlayer.y);

            if (length != 0) {
                directionToPlayer /= length; // Normalize the direction
            }

            // Dot product to check if the player is in the line of fire
            float dotProduct = enemyDirection.x * directionToPlayer.x + enemyDirection.y * directionToPlayer.y;

            if (dotProduct > 0.98f && length < 700.0f) { // 0.98f is close to 1.0f, meaning almost the same direction
                // The player is within the line of fire
                if(playerHealth.health > 0){
                  playerHealth.health -= 25; // Reduce player health by 25
                  std::cout << "Player hit! Health reduced by 25." << std::endl;
                  if(playerHealth.health <= 0){
                    playerState.isAlive = false;
                  }
                }
            } else {
                std::cout << "Player missed!" << std::endl;
            }
              enemy.state = Chase;
              enemy.attackingTimer = 0.0f;
            }
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