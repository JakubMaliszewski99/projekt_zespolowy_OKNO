#pragma once
#include "../../components/MinimapSpriteComponent.h"
#include "../../components/TransformComponent.h"
#include "../include/core/core.h"
#include "../include/core/ecs/ECSManager.h"
#include "../include/core/math/utilities.h"
#include "core/engine/BSP.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <utility>

class MinimapRenderingSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager,
            std::shared_ptr<sf::RenderWindow> renderWindow,
            std::shared_ptr<BSP> bsp, Entity playerEntity,
            DebugSettings debugSettings) {
    m_manager = manager;
    m_renderWindow = renderWindow;
    m_bsp = bsp;
    m_playerEntity = playerEntity;
    m_debugSettings = debugSettings;
  }

  void update(float dt) {

    m_renderWindow->clear();
    for (auto const &entity : m_entities) {
      auto &transform = m_manager->getComponent<TransformComponent>(entity);
      auto &drawable = m_manager->getComponent<MinimapSpriteComponent>(entity);

      drawable.camera.setCenter(
          sf::Vector2f(transform.positionX, transform.positionY));
      drawable.sprite->update(transform);

      if (drawable.isPlayer) {
        drawable.camera.setSize(WIDTH, -HEIGHT);
        m_renderWindow->setView(drawable.camera);
      }

      m_renderWindow->draw(*drawable.sprite);
    }

    TransformComponent playerTransform =
        m_manager->getComponent<TransformComponent>(m_playerEntity);
    if (m_debugSettings.displayVisibleNodes) {
      drawAllNodes(playerTransform.positionX, playerTransform.positionY,
                   RAD2DEG(playerTransform.angle), m_bsp->m_rootNodeID);
    }
    if (m_debugSettings.displayVisibleSegments) {
      renderBSP(playerTransform.positionX, playerTransform.positionY,
                RAD2DEG(playerTransform.angle), m_bsp->m_rootNodeID);
    }
    // m_renderWindow->display();
  }

private:
  std::shared_ptr<sf::RenderWindow> m_renderWindow;
  std::shared_ptr<ECSManager> m_manager;
  std::shared_ptr<BSP> m_bsp;
  Entity m_playerEntity;
  DebugSettings m_debugSettings;

  void renderSegment(GameLevelSegment segment) {
    auto v1 = m_bsp->m_gameLevel->vertexes[segment.startingVectorNumber];
    auto v2 = m_bsp->m_gameLevel->vertexes[segment.endingVectorNumber];

    sf::Vector2f start(v1.x, v1.y);
    sf::Vector2f end(v2.x, v2.y);

    sf::VertexArray line(sf::Lines, 3);
    line[0].position = start;
    line[1].position = end;
    line[0].color = sf::Color::Green;
    line[1].color = sf::Color::Green;

    m_renderWindow->draw(line);
  }

  void renderSubSector(float playerX, float playerY, float playerAngle,
                       int16_t ssectorID) {
    auto ssector = m_bsp->m_gameLevel->subsuctors[ssectorID];
    for (int i = 0; i < ssector.segCount; i++) {
      auto segment =
          m_bsp->m_gameLevel->segments[ssector.firstSectorNumber + i];
      auto start = m_bsp->m_gameLevel->vertexes[segment.startingVectorNumber];
      auto end = m_bsp->m_gameLevel->vertexes[segment.endingVectorNumber];
      auto cullingResults = backfaceCullingSegment(
          playerX, playerY, playerAngle, convertMapVertexToVector(start),
          convertMapVertexToVector(end));
      if (std::get<0>(cullingResults)) {
        renderSegment(segment);
      }
    }
  }

  void renderBSP(float playerX, float playerY, float playerAngle,
                 int16_t nodeID) {
    if (nodeID & SSECTOR_ID) {
      int ssectorID = nodeID - SSECTOR_ID;
      renderSubSector(playerX, playerY, playerAngle, ssectorID);
      return;
    }

    // right == front
    // left == back
    auto node = m_bsp->m_gameLevel->nodes[nodeID];
    if (m_bsp->isPlayerOnBackSide(playerX, playerY, nodeID)) {
      renderBSP(playerX, playerY, playerAngle, node.leftChild);
      if (checkBoundingBox(playerX, playerY, playerAngle,
                           node.rightBoundingBox[0], node.rightBoundingBox[1],
                           node.rightBoundingBox[2],
                           node.rightBoundingBox[3])) {

        renderBSP(playerX, playerY, playerAngle, node.rightChild);
      }
    } else {
      renderBSP(playerX, playerY, playerAngle, node.rightChild);
      if (checkBoundingBox(playerX, playerY, playerAngle,
                           node.leftBoundingBox[0], node.leftBoundingBox[1],
                           node.leftBoundingBox[2], node.leftBoundingBox[3])) {
        renderBSP(playerX, playerY, playerAngle, node.leftChild);
      }
    }
  }

  // ONLY FOR DEBUG PURPOSE
  void drawAllNodes(float playerX, float playerY, float playerAngle,
                    int16_t nodeID) {
    if (nodeID & SSECTOR_ID) {
      return;
    }

    drawNode(nodeID);
    auto node = m_bsp->m_gameLevel->nodes[nodeID];
    if (m_bsp->isPlayerOnBackSide(playerX, playerY, nodeID)) {
      drawAllNodes(playerX, playerY, playerAngle, node.leftChild);
      if (checkBoundingBox(playerX, playerY, playerAngle,
                           node.rightBoundingBox[0], node.rightBoundingBox[1],
                           node.rightBoundingBox[2],
                           node.rightBoundingBox[3])) {

        drawAllNodes(playerX, playerY, playerAngle, node.rightChild);
      }
    } else {
      drawAllNodes(playerX, playerY, playerAngle, node.rightChild);
      if (checkBoundingBox(playerX, playerY, playerAngle,
                           node.leftBoundingBox[0], node.leftBoundingBox[1],
                           node.leftBoundingBox[2], node.leftBoundingBox[3])) {
        drawAllNodes(playerX, playerY, playerAngle, node.leftChild);
      }
    }
  }

  void drawNode(int16_t nodeID) {
    auto node = m_bsp->m_gameLevel->nodes[nodeID];
    auto boundingBoxFront = node.leftBoundingBox;
    auto boundingBoxBack = node.rightBoundingBox;
    drawBoundingBox(boundingBoxFront[0], boundingBoxFront[1],
                    boundingBoxFront[2], boundingBoxFront[3], sf::Color::Red);
    drawBoundingBox(boundingBoxBack[0], boundingBoxBack[1], boundingBoxBack[2],
                    boundingBoxBack[3], sf::Color::Green);
    sf::Vector2f start(node.xStart, node.yStart);
    sf::Vector2f end(node.xStart + node.xEndOffset,
                     node.yStart + node.yEndOffset);

    sf::VertexArray line(sf::Lines, 3);
    line[0].position = start;
    line[1].position = end;
    line[0].color = sf::Color::Blue;
    line[1].color = sf::Color::Blue;

    m_renderWindow->draw(line);
  }

  void drawBoundingBox(int16_t top, int16_t bottom, int16_t left, int16_t right,
                       sf::Color color) {
    sf::RectangleShape rect;
    int16_t x = left;
    int16_t y = top;
    int16_t width = right - x;
    int16_t height = bottom - y;
    rect.setPosition(sf::Vector2f(x, y));
    rect.setSize(sf::Vector2f(width, height));
    rect.setOutlineThickness(5);
    rect.setOutlineColor(color);
    rect.setFillColor(sf::Color(0, 0, 0, 0));
    m_renderWindow->draw(rect);
  }
};
