#pragma once
#include<chrono>
#include "../../core/core.h"
#include "../../core/ecs/ECSManager.h"
#include "../../core/ecs/System.h"
#include "../../core/engine/BSP.h"
#include "../../core/engine/GameLevel.h"
#include "../../core/engine/InputManager.h"
#include "../../core/math/collisions.h"
#include "../components/ControllableComponent.h"
#include "../components/MinimapSpriteComponent.h"
#include "../components/TransformComponent.h"
#include "../components/PlayerStateComponent.h"

const float PLAYER_ACCELERATION = 700.0f;
const float PLAYER_MAX_SPEED = 360.0f;
const float DEFAULT_ROTATION_SPEED = 2.4f;
const float DAMPING_FACTOR = 100.0f;
const float DAMPING_TIME_COEFFICIENT = -2.0f;
const float INTERPOLATION_FACTOR = 4.5f;
const float FALLING_SPEED = 500.0f;
const float CLIMBING_SPEED = 50.0f;
const float EPSILON = 5.0f;

class PlayerMovementSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager, std::shared_ptr<BSP> bsp) {
    m_manager = manager;
    m_bsp = bsp;
  }

  void update(float dt) {
    for (auto const &entity : m_entities) {
      auto &transform = m_manager->getComponent<TransformComponent>(entity);
      auto &controllable = m_manager->getComponent<ControllableComponent>(entity);
      auto &drawable = m_manager->getComponent<MinimapSpriteComponent>(entity);
      auto &state = m_manager->getComponent<PlayerStateComponent>(entity);

      if (!controllable.isPlayer) {
        continue;
      }
      //reset acc to 0
      sf::Vector2f acceleration = {0.0f, 0.0f};

      //prepare angles
      float cosAngle = cos(transform.angle);
      float sinAngle = sin(transform.angle);
      float cosAngle90 = cos(transform.angle + 90 * (M_PI / 180));
      float sinAngle90 = sin(transform.angle + 90 * (M_PI / 180));

      //movement
      if(state.isMovingForward){
        if(InputManager::getInstance()->getMouseDelta().y < 0){
          acceleration.x += cosAngle * PLAYER_ACCELERATION/500 * -InputManager::getInstance()->getMouseDelta().y;
          acceleration.y += sinAngle * PLAYER_ACCELERATION/500 * -InputManager::getInstance()->getMouseDelta().y;
        } else {
          acceleration.x += cosAngle * PLAYER_ACCELERATION * dt;
          acceleration.y += sinAngle * PLAYER_ACCELERATION * dt;
        }
      }
      if(state.isMovingBackwards){
        if(InputManager::getInstance()->getMouseDelta().y > 0){
          acceleration.x -= cosAngle * PLAYER_ACCELERATION/500 * InputManager::getInstance()->getMouseDelta().y;
          acceleration.y -= sinAngle * PLAYER_ACCELERATION/500 * InputManager::getInstance()->getMouseDelta().y;
        }else{
          acceleration.x -= cosAngle * PLAYER_ACCELERATION * dt;
          acceleration.y -= sinAngle * PLAYER_ACCELERATION * dt;
        }
      }
      if(state.isMovingRight){
        if(InputManager::getInstance()->getMouseDelta().x > 0){
          acceleration.x -= cosAngle90 * PLAYER_ACCELERATION/500 * InputManager::getInstance()->getMouseDelta().x;
          acceleration.y -= sinAngle90 * PLAYER_ACCELERATION/500 * InputManager::getInstance()->getMouseDelta().x;
        }else{
          acceleration.x -= cosAngle90 * PLAYER_ACCELERATION * dt;
          acceleration.y -= sinAngle90 * PLAYER_ACCELERATION * dt;
        }
      }
      if(state.isMovingLeft){
        if(InputManager::getInstance()->getMouseDelta().x < 0){
          acceleration.x += cosAngle90 * PLAYER_ACCELERATION/500 * -InputManager::getInstance()->getMouseDelta().x;
          acceleration.y += sinAngle90 * PLAYER_ACCELERATION/500 * -InputManager::getInstance()->getMouseDelta().x;
        }else{
          acceleration.x += cosAngle90 * PLAYER_ACCELERATION * dt;
          acceleration.y += sinAngle90 * PLAYER_ACCELERATION * dt;
        }
      }

      // Tˆumienie gdy gracz przestaje si© rusza†
      if (!state.isMovingForward && !state.isMovingBackwards && !state.isMovingRight && !state.isMovingLeft) {
        float dampingFactor = std::pow(DAMPING_FACTOR, dt * DAMPING_TIME_COEFFICIENT);
        transform.velocity.x *= dampingFactor;
        transform.velocity.y *= dampingFactor;
      }else{
        transform.velocity += acceleration;
      }

      // Interpolacja wektora pr©dko˜ci
      if (std::sqrt(acceleration.x * acceleration.x + acceleration.y * acceleration.y) > 0.0f) {
        sf::Vector2f targetVelocity = normalize(acceleration) * std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y);
        transform.velocity = lerp(transform.velocity, targetVelocity, INTERPOLATION_FACTOR * dt);
      }

       // Clamp velocity to max speed
      if (std::sqrt(transform.velocity.x * transform.velocity.x + transform.velocity.y * transform.velocity.y) > PLAYER_MAX_SPEED) {
        transform.velocity = normalize(transform.velocity) * PLAYER_MAX_SPEED;
      }

      // Apply velocity to position
      transform.positionX += transform.velocity.x * dt;
      transform.positionY += transform.velocity.y * dt;

      //Save the acceleration value to transform component - Is it necessary tho?
      transform.acceleration = acceleration;

      //rotation
      if(state.isRotatingRight){
        if(InputManager::getInstance()->getMouseDelta().x > 0){
          transform.angle -= DEFAULT_ROTATION_SPEED/1000 * InputManager::getInstance()->getMouseDelta().x;
        }else{
          transform.angle -= DEFAULT_ROTATION_SPEED * dt;
        }
      }
      if(state.isRotatingLeft){
        if(InputManager::getInstance()->getMouseDelta().x < 0){
          transform.angle += DEFAULT_ROTATION_SPEED/1000 * -InputManager::getInstance()->getMouseDelta().x;
        }else{
          transform.angle += DEFAULT_ROTATION_SPEED * dt;
        }
      }
      /*

      // TODO: Velocity feedback??
      // Kudos to Poke Dev: https://www.youtube.com/watch?v=YR6Q7dUz2uk
      int16_t subsectorID = m_bsp->getCurrentSubsectorID(transform.positionX,
                                                         transform.positionY);
      auto subsector = m_bsp->m_gameLevel->subsuctors[subsectorID];

      for (int i = subsector.firstSectorNumber;
           i < subsector.firstSectorNumber + subsector.segCount; i++) {
        auto segment = m_bsp->m_gameLevel->segments[i];
        auto line = m_bsp->m_gameLevel->linedefs[segment.linedefNumber];
        auto startVertex = m_bsp->m_gameLevel->vertexes[line.startVertex];
        auto endVertex = m_bsp->m_gameLevel->vertexes[line.endVertex];
        auto frontsector = m_bsp->m_gameLevel->sectors[segment.frontSector];
        auto backSectorId = segment.backSector;

        if (backSectorId != -1) {
          auto backSector = m_bsp->m_gameLevel->sectors[segment.backSector];
          if (backSector.floorHeight <
              transform.positionZ - PLAYER_HEIGHT / 2) {
            continue;
          }
        }

        // TODO: Fix: Projection should stop on the wall, problem occures in 3d
        // mode
        // TODO: Fix corners, probably recurssion is required
        // TODO: Fix when falling higher wall, the player is stuck
        float projectedX = transform.positionX + velocity.x;
        float projectedY = transform.positionY + velocity.y;
        sf::Vector2f hit;
        if (lineCircleCollision(startVertex.x, startVertex.y, endVertex.x,
                                endVertex.y, projectedX, projectedY, 1.25f,
                                hit)) {
          velocity = projectVectorOntoLine(
              velocity, sf::Vector2f(endVertex.x - startVertex.x,
                                     endVertex.y - startVertex.y));
        }
      }

      transform.velocity = normalize(velocity);
      */
     
      // TODO: We don't really need targetPositionZ because it's always
      // floorHeight
      int16_t floorHeight = m_bsp->getSubSectorHeight(transform.positionX, transform.positionY);
      //transform.targetPositonZ = floorHeight + PLAYER_HEIGHT;
      float dHeight = floorHeight + PLAYER_HEIGHT - transform.positionZ;
      
      // TODO: Maybe it should be added to velocity?? And make it 3D
      // Player should be accelerating when climbing and falling
      // but when climbing it should get a some constant
      // but when falling it should be related to gravity!
      if (abs(dHeight) < EPSILON) {
        transform.positionZ = floorHeight + PLAYER_HEIGHT;
        //transform.velocityZ = 0;
      } else if (dHeight > 0) {
        transform.positionZ += CLIMBING_SPEED * dt;

      } else if (dHeight < 0) {
        transform.positionZ -= FALLING_SPEED * dt;
      }
    }
  }

private:
  std::shared_ptr<ECSManager> m_manager;
  std::shared_ptr<BSP> m_bsp;
};