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
const float GRAVITY = 1000.0f;
const float CLIMBING_SPEED = 100.0f;
const float MAX_FALLING_SPEED = -600.0f;
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


       // Calculate the projected position based on current velocity
        sf::Vector2f projectedPosition = {
            transform.positionX + transform.velocity.x * dt,
            transform.positionY + transform.velocity.y * dt
        };

        // Now use projectedPosition in collision detection
        bool collisionDetected = false;
        sf::Vector2f correctionVector = {0.0f, 0.0f};
        sf::Vector2f totalCorrectionVector = {0.0f, 0.0f};
        sf::Vector2f wallNormal = {0.0f, 0.0f};

        int16_t subsectorID = m_bsp->getCurrentSubsectorID(transform.positionX, transform.positionY);
        auto subsector = m_bsp->m_gameLevel->subsuctors[subsectorID];
/*
        for (int i = subsector.firstSectorNumber; i < subsector.firstSectorNumber + subsector.segCount; i++) {
            auto segment = m_bsp->m_gameLevel->segments[i];
            auto line = m_bsp->m_gameLevel->linedefs[segment.linedefNumber];
            auto startVertex = m_bsp->m_gameLevel->vertexes[line.startVertex];
            auto endVertex = m_bsp->m_gameLevel->vertexes[line.endVertex];
            auto frontSector = m_bsp->m_gameLevel->sectors[segment.frontSector];
            int backSectorId = segment.backSector;

            //Obliczenie r¢znicy wysokosci miedzy sektorami
            auto backSector = m_bsp->m_gameLevel->sectors[backSectorId];
            int16_t backSectorHeight = backSector.floorHeight;
            int16_t frontSectorHeight = frontSector.floorHeight;
            float heightDifference = backSectorHeight - frontSectorHeight;

            sf::Vector2f hitPoint;
            if (lineCircleCollision(
                    startVertex.x, startVertex.y,
                    endVertex.x, endVertex.y,
                    projectedPosition.x, projectedPosition.y,
                    25.0f, hitPoint) && (backSectorId == -1 || heightDifference >= 24)) {

                // If a collision is detected, calculate a correction vector
                sf::Vector2f wallVector = {(float)endVertex.x - (float)startVertex.x, (float)endVertex.y - (float)startVertex.y};
                wallNormal = normalize({wallVector.y, -wallVector.x});

                

                collisionDetected = true;                
                correctionVector = transform.velocity - dot(transform.velocity, wallNormal) * wallNormal;
                //correctionVector = projectVectorOntoLine(transform.velocity, wallVector);
                totalCorrectionVector += correctionVector;

            }
        }

        // If a collision was detected, adjust the velocity and position
        if (collisionDetected) {
            // Normalize the total correction vector if necessary
            if (std::sqrt(totalCorrectionVector.x * totalCorrectionVector.x + totalCorrectionVector.y * totalCorrectionVector.y) > 1.0f) {
                totalCorrectionVector = normalize(totalCorrectionVector);
            }
            

            transform.velocity = totalCorrectionVector;
            transform.positionX += transform.velocity.x * dt;
            transform.positionY += transform.velocity.y * dt;

            // Ensure the player is slightly moved away from the wall
            transform.positionX += wallNormal.x * 0.1f;
            transform.positionY += wallNormal.y * 0.1f;
        } else {
            // If no collision, proceed with normal movement
            transform.positionX = projectedPosition.x;
            transform.positionY = projectedPosition.y;
        }
        */

       for (int i = subsector.firstSectorNumber; i < subsector.firstSectorNumber + subsector.segCount; i++) {
    auto segment = m_bsp->m_gameLevel->segments[i];
    auto line = m_bsp->m_gameLevel->linedefs[segment.linedefNumber];
    auto startVertex = m_bsp->m_gameLevel->vertexes[line.startVertex];
    auto endVertex = m_bsp->m_gameLevel->vertexes[line.endVertex];
    auto frontSector = m_bsp->m_gameLevel->sectors[segment.frontSector];
    int backSectorId = segment.backSector;

    // Calculate height difference between sectors
    auto backSector = m_bsp->m_gameLevel->sectors[backSectorId];
    int16_t backSectorHeight = backSector.floorHeight;
    int16_t frontSectorHeight = frontSector.floorHeight;
    float heightDifference = backSectorHeight - frontSectorHeight;

    sf::Vector2f hitPoint;
    if (lineCircleCollision(
            startVertex.x, startVertex.y,
            endVertex.x, endVertex.y,
            projectedPosition.x, projectedPosition.y,
            25.0f, hitPoint) && (backSectorId == -1 || heightDifference >= 24)) {

        // Calculate wall vector and normal
        sf::Vector2f wallVector = {(float)endVertex.x - (float)startVertex.x, (float)endVertex.y - (float)startVertex.y};
        sf::Vector2f wallNormal = normalize({wallVector.y, -wallVector.x});

        // Project velocity onto the wall to prevent moving into it
        transform.velocity -= dot(transform.velocity, wallNormal) * wallNormal;

        // Slightly adjust position to avoid overlap with the wall
        sf::Vector2f correctionStep = wallNormal * 0.1f; // Small step away from the wall
        transform.positionX += correctionStep.x;
        transform.positionY += correctionStep.y;

        collisionDetected = true; 
    }
}

if (!collisionDetected) {
    // If no collision, proceed with normal movement
    transform.positionX = projectedPosition.x;
    transform.positionY = projectedPosition.y;
} else {
    // Apply the velocity to position with the corrected velocity
    transform.positionX += transform.velocity.x * dt;
    transform.positionY += transform.velocity.y * dt;
}
       
     //Wspinanie/opadanie
      int16_t currentFloorHeight = m_bsp->getSubSectorHeight(transform.positionX, transform.positionY);
      float dHeight = currentFloorHeight + PLAYER_HEIGHT - transform.positionZ;

      if(abs(dHeight) > EPSILON){
        if (dHeight > 0){
          transform.velocityZ = CLIMBING_SPEED;
        } else if (dHeight < 0){
          transform.velocityZ -= GRAVITY * dt;
          //Ograniczenie predkosci spadania
          if (transform.velocityZ < MAX_FALLING_SPEED) {
            transform.velocityZ = MAX_FALLING_SPEED;
          }
        }
      }else{
        transform.positionZ = currentFloorHeight + PLAYER_HEIGHT;
        transform.velocityZ = 0;
      }

      transform.positionZ += transform.velocityZ * dt;
    }
  }

private:
  std::shared_ptr<ECSManager> m_manager;
  std::shared_ptr<BSP> m_bsp;
};