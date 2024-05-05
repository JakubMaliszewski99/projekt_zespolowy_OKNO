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


//private:
  GameLevel *m_gameLevel;
  int m_rootNodeID;
};