#pragma once
#include "core/engine/GameLevel.h"
#include <iostream>

const int16_t SSECTOR_ID = 0x8000;

class BSP {
public:
  BSP(GameLevel *gameLevel) {
    m_rootNodeID = gameLevel->nodes.size() - 1;
    m_gameLevel = gameLevel;
  }

  bool isPlayerOnBackSide(float playerX, float playerY, int16_t nodeID) {
    auto node = m_gameLevel->nodes[nodeID];
    float dx = playerX - node.xStart;
    float dy = playerY - node.yStart;
    float cross = dx * node.yEndOffset - dy * node.xEndOffset;

    return cross <= 0;
  }

  int16_t getCurrentSubsectorID(float playerX, float playerY) {
    int nodeID = m_rootNodeID;
    while (!(nodeID & SSECTOR_ID)) {
      // right == front
      // left == back
      auto node = m_gameLevel->nodes[nodeID];
      if (isPlayerOnBackSide(playerX, playerY, nodeID)) {
        nodeID = node.leftChild;
      } else {
        nodeID = node.rightChild;
      }
    }

    return nodeID - SSECTOR_ID;
  }

  int16_t getSubSectorHeight(float playerX, float playerY) {
    int16_t subsectorID = getCurrentSubsectorID(playerX, playerY);

    auto subsector = m_gameLevel->subsuctors[subsectorID];
    auto segment = m_gameLevel->segments[subsector.firstSectorNumber];
    auto frontSector = m_gameLevel->sectors[segment.frontSector];
    return frontSector.floorHeight;
  }

  // private:
  GameLevel *m_gameLevel;
  int m_rootNodeID;
};
