#pragma once
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

const float FALLING_SPEED = 500.0f;
const float CLIMBING_SPEED = 50.0f;
const float EPSILON = 5.0f;
const float PLAYERSPEED = 500.0f;
const float DEFAULT_ROTATION_SPEED = 10;

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

      //movement
      if(state.isMovingForward){
        transform.positionX += cos(transform.angle) * PLAYERSPEED * dt;
        transform.positionY += sin(transform.angle) * PLAYERSPEED * dt;
        //std::cout << "Moving Forward";
      }
      if(state.isMovingBackwards){
        transform.positionX -= cos(transform.angle) * PLAYERSPEED * dt;
        transform.positionY -= sin(transform.angle) * PLAYERSPEED * dt;
        //std::cout << "Moving Backwards";
      }
      if(state.isMovingRight){
        transform.positionX -= cos(transform.angle + 90 * (M_PI / 180)) * PLAYERSPEED * dt;
        transform.positionY -= sin(transform.angle + 90 * (M_PI / 180)) * PLAYERSPEED * dt;
        //std::cout << "Moving Right";
      }
      if(state.isMovingLeft){
        transform.positionX += cos(transform.angle + 90 * (M_PI / 180)) * PLAYERSPEED * dt;
        transform.positionY += sin(transform.angle + 90 * (M_PI / 180)) * PLAYERSPEED * dt;
        //std::cout <<"Moving Left";
      }

      //rotation
      if(state.isRotatingRight){
        transform.angle -= DEFAULT_ROTATION_SPEED * dt * 0.4f;
        //std::cout << "Rotating Right";
      }
      if(state.isRotatingLeft){
        transform.angle += DEFAULT_ROTATION_SPEED * dt * 0.4f;
        //std::cout << "Rotating Left";
      }

      sf::Vector2f velocity = transform.velocity;

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
      //transform.positionX += velocity.x;
      //transform.positionY += velocity.y;

      // TODO: We don't really need targetPositionZ because it's always
      // floorHeight
      int16_t floorHeight =
          m_bsp->getSubSectorHeight(transform.positionX, transform.positionY);
      transform.targetPositonZ = floorHeight + PLAYER_HEIGHT;
      float dHeight = transform.targetPositonZ - transform.positionZ;
      // TODO: Maybe it should be added to velocity?? And make it 3D
      // Player should be accelerating when climbing and falling
      // but when climbing it should get a some constant
      // but when falling it should be related to gravity!
      if (abs(dHeight) < EPSILON) {
        transform.positionZ = transform.targetPositonZ;
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