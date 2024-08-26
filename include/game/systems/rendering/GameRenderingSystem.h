#pragma once
#include "../../components/GameDrawableComponent.h"
#include "../../components/PlayerStateComponent.h"
#include "../../components/TransformComponent.h"
#include "../include/core/core.h"
#include "../include/core/ecs/ECSManager.h"
#include "../include/core/math/utilities.h"
#include "core/engine/BSP.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstring>
#include <random>
#include <set>
#include <utility>

const float HEAD_BOBBING_FREQUENCY = 10.0f;
const float HEAD_BOBBING_AMPLITUDE = 7.5f;

class GameRenderingSystem : public System {
public:
  void init(std::shared_ptr<ECSManager> manager,
            std::shared_ptr<sf::RenderWindow> renderWindow,
            std::shared_ptr<BSP> bsp, Entity playerEntity) {
    m_manager = manager;
    m_renderWindow = renderWindow;
    m_bsp = bsp;
    m_playerEntity = playerEntity;
    for (int i = 0; i < WIDTH; i++) {
      m_upperClip.push_back(-1);
      m_lowerClip.push_back(HEIGHT);

      float angle = RAD2DEG(atanf((H_WIDTH - i) / SCREEN_DIST));
      m_x2Angle.push_back(angle);
    }
    m_defaultView = sf::View(sf::FloatRect(0, 0, WIDTH, HEIGHT));
    m_frameBuffer.create(WIDTH, HEIGHT);
    m_texture.create(WIDTH, HEIGHT);
  }

  void update(float dt) {
    // TODO: Is clear necessary? Maybe copy will be faster?
    m_screenSet.clear();
    m_upperClip.clear();
    m_lowerClip.clear();
    for (int i = 0; i < WIDTH; i++) {
      m_screenSet.insert(i);
      m_upperClip.push_back(-1);
      m_lowerClip.push_back(HEIGHT);
    }

    m_playerTransform =
        m_manager->getComponent<TransformComponent>(m_playerEntity);
    m_playerState =
        m_manager->getComponent<PlayerStateComponent>(m_playerEntity);

    // TODO:Move it somewhere else? + Make the headbbob more smooth
    m_playerHeight = m_playerTransform.positionZ;
    // Head bobbing effect
    if (m_playerState.isMovingForward || m_playerState.isMovingBackwards ||
        m_playerState.isMovingRight || m_playerState.isMovingLeft) {
      sf::Time elapsed = m_headBobbClock.getElapsedTime();
      float time = elapsed.asSeconds();
      m_playerHeight +=
          std::sin(time * HEAD_BOBBING_FREQUENCY) * HEAD_BOBBING_AMPLITUDE *
          (std::sqrt(
               m_playerTransform.velocity.x * m_playerTransform.velocity.x +
               m_playerTransform.velocity.y * m_playerTransform.velocity.y) /
           PLAYER_MAX_SPEED);
    }

    renderBSP(m_playerTransform.positionX, m_playerTransform.positionY,
              RAD2DEG(m_playerTransform.angle), m_bsp->m_rootNodeID);

    m_texture.update(m_frameBuffer);
    m_screenSprite.setTexture(m_texture);

    m_renderWindow->setView(m_defaultView);
    m_renderWindow->clear();
    m_renderWindow->draw(m_screenSprite);
  }

private:
  std::shared_ptr<sf::RenderWindow> m_renderWindow;
  std::shared_ptr<ECSManager> m_manager;
  std::shared_ptr<BSP> m_bsp;
  TransformComponent m_playerTransform;
  PlayerStateComponent m_playerState;
  Entity m_playerEntity;
  std::set<int> m_screenSet;
  std::vector<int> m_upperClip;
  std::vector<int> m_lowerClip;
  std::vector<float> m_x2Angle;
  sf::View m_defaultView;
  sf::Image m_frameBuffer;
  sf::Sprite m_screenSprite;
  sf::Texture m_texture;
  sf::Clock m_headBobbClock;
  int m_playerHeight;

  std::unordered_map<std::string, sf::Color> colorMapping;

  sf::Color getColor(int8_t *texture, int lightLevel) {
    std::string key((char *)texture);
    if (colorMapping.count(key) == 0) {
      sf::Color color = getRandomColor();

      color.r *= lightLevel / 255.0f;
      color.g *= lightLevel / 255.0f;
      color.b *= lightLevel / 255.0f;
      colorMapping[key] = getRandomColor();
      return colorMapping[key];
    } else {
      sf::Color color = colorMapping[key];
      color.r *= lightLevel / 255.0f;
      color.g *= lightLevel / 255.0f;
      color.b *= lightLevel / 255.0f;
      return color;
    }
  }

  void renderVline(float x, float y1, float y2, sf::Color color) {
    if (y1 > y2) {
      return;
    }
    for (int y = y1; y < y2; y++) {
      m_frameBuffer.setPixel(x, y, color);
    }
  }

  void renderWallColumn(float x, float y1, float y2, int texAlt, int texCol,
                        float invScale, int lightLevel,
                        const std::unique_ptr<GameLevelTexture> &texture) {
    if (y1 > y2) {
      return;
    }

    int textureW = texture->width;
    int textureH = texture->height;
    int textureCol = negMod(texCol, textureW);
    float textureY = texAlt + ((float)y1 - (float)H_HEIGHT) * invScale;

    for (int y = y1; y < y2 + 1; y++) {
      color_t color = texture->image[textureCol][negMod(textureY, textureH)];
      float fLightLevel = lightLevel / 255.0f;
      sf::Color sfColor{color.r, color.g, color.b};
      sfColor.r *= lightLevel / 255.0f;
      sfColor.g *= lightLevel / 255.0f;
      sfColor.b *= lightLevel / 255.0f;
      m_frameBuffer.setPixel(x, y, sfColor);
      textureY += invScale;
    }
  }

  void renderFlat(float x, float y1, float y2, float worldZ, int lightLevel,
                  const std::unique_ptr<GameLevelTexture> &texture) {
    if (y1 > y2) {
      return;
    }

    float player_dir_x = cosf(m_playerTransform.angle);
    float player_dir_y = sinf(m_playerTransform.angle);
    uint8_t *framebufferPtr = (uint8_t *)m_frameBuffer.getPixelsPtr();
    for (int y = y1; y < y2; y++) {
      float z = H_WIDTH * worldZ / (H_HEIGHT - y);

      float px = player_dir_x * z + m_playerTransform.positionX;
      float py = player_dir_y * z + m_playerTransform.positionY;

      float left_x = -player_dir_y * z + px;
      float left_y = player_dir_x * z + py;
      float right_x = player_dir_y * z + px;
      float right_y = -player_dir_x * z + py;

      float dx = (right_x - left_x) / WIDTH;
      float dy = (right_y - left_y) / WIDTH;

      float tx = int(left_x + dx * x) & 63;
      float ty = int(left_y + dy * x) & 63;

      color_t color = texture->image[tx][ty];
      sf::Color sfColor{color.r, color.g, color.b};
      sfColor.r *= lightLevel / 255.0f;
      sfColor.g *= lightLevel / 255.0f;
      sfColor.b *= lightLevel / 255.0f;
      m_frameBuffer.setPixel(x, y, sfColor);
    }
  }

  // TODO: Move it to BSP
  void drawSolidWallRange(GameLevelSegment segment, float x1, float x2,
                          float inAngle) {
    auto frontSector = m_bsp->m_gameLevel->sectors[segment.frontSector];
    auto line = m_bsp->m_gameLevel->linedefs[segment.linedefNumber];
    auto frontside = m_bsp->m_gameLevel->sidedefs[line.frontSidedef];

    auto lightLevel = frontSector.lightLevel;

    float worldFrontZ1 = frontSector.ceilingHeight - m_playerHeight;
    float worldFrontZ2 = frontSector.floorHeight - m_playerHeight;

    bool isWallVisible =
        strncmp((const char *)frontside.middleTextureName, "-", 1) != 0;
    bool isCeilVisible = worldFrontZ1 > 0;
    bool isFloorVisible = worldFrontZ2 < 0;

    float rwNormalAngle = segment.angle + 90;
    float offsetAngle = rwNormalAngle - inAngle;

    auto startingVertex =
        m_bsp->m_gameLevel->vertexes[segment.startingVectorNumber];
    float dx = m_playerTransform.positionX - startingVertex.x;
    float dy = m_playerTransform.positionY - startingVertex.y;
    float hypotenuse = sqrt(dx * dx + dy * dy);
    float rwDistance = hypotenuse * cosf(DEG2RAD(offsetAngle));

    float rwScaleStep = 0;
    float rwScale1 = scaleFromGlobalAngle(x1, rwNormalAngle, rwDistance,
                                          RAD2DEG(m_playerTransform.angle));

    int offsetAngleMod = (int)fmodf(rwScale1, 360);
    if (offsetAngleMod - 1 == 90 || offsetAngleMod + 1 == 90) {
      rwScale1 *= 0.1;
    }

    if (x1 < x2) {
      float scale2 = scaleFromGlobalAngle(x2, rwNormalAngle, rwDistance,
                                          RAD2DEG(m_playerTransform.angle));
      rwScaleStep = (scale2 - rwScale1) / (x2 - x1);
    }

    std::unique_ptr<GameLevelTexture> &texture =
        m_bsp->m_gameLevel->textureImages[std::string(
            (char *)frontside.middleTextureName, 8)];
    std::unique_ptr<GameLevelTexture> &ceilGameTexture =
        m_bsp->m_gameLevel->flatImages[std::string(
            (char *)frontSector.ceilingTextureName, 8)];
    std::unique_ptr<GameLevelTexture> &floorGameTexture =
        m_bsp->m_gameLevel
            ->flatImages[std::string((char *)frontSector.floorTextureName, 8)];

    float vTop = 0;
    float middleTextAlt = 0;
    if ((line.flags & 0x0010) > 0) {
      vTop = frontSector.floorHeight + texture->height;
      middleTextAlt = vTop - m_playerHeight;
    } else {
      middleTextAlt = worldFrontZ1;
    }
    middleTextAlt += frontside.yOffset;

    float rwOffset = hypotenuse * sinf(DEG2RAD(offsetAngle));
    rwOffset += segment.offset + frontside.xOffset;
    float rwCenterAngle = rwNormalAngle - RAD2DEG(m_playerTransform.angle);

    float wallY1 = H_HEIGHT - worldFrontZ1 * rwScale1;
    float wallY1Step = -rwScaleStep * worldFrontZ1;
    float wallY2 = H_HEIGHT - worldFrontZ2 * rwScale1;
    float wallY2Step = -rwScaleStep * worldFrontZ2;

    for (int x = x1; x < x2 + 1; x++) {
      float drawWallY1 = wallY1 - 1;
      float drawWallY2 = wallY2;

      if (isCeilVisible) {
        int cy1 = m_upperClip[x] + 1;
        int cy2 = std::min<int>(drawWallY1 - 1, m_lowerClip[x] - 1);
        renderFlat(x, cy1, cy2, worldFrontZ1, lightLevel, ceilGameTexture);
      }

      if (isWallVisible) {
        int wy1 = std::max<int>(drawWallY1, m_upperClip[x] + 1);
        int wy2 = std::min<int>(drawWallY2, m_lowerClip[x] - 1);
        if (wy1 < wy2) {
          float angle = rwCenterAngle - m_x2Angle[x];
          float textureColumn = rwDistance * tanf(DEG2RAD(angle)) - rwOffset;
          float invScale = 1.0 / rwScale1;
          renderWallColumn(x, wy1, wy2, middleTextAlt, textureColumn, invScale,
                           lightLevel, texture);
        }
      }

      if (isFloorVisible) {
        int fy1 = std::max<int>(drawWallY2 + 1, m_upperClip[x] + 1);
        int fy2 = m_lowerClip[x] - 1;
        renderFlat(x, fy1, fy2, worldFrontZ2, lightLevel, ceilGameTexture);
      }

      rwScale1 += rwScaleStep;
      wallY1 += wallY1Step;
      wallY2 += wallY2Step;
    }
  }

  void drawPortalWallRange(GameLevelSegment segment, float x1, float x2,
                           float inAngle) {
    auto frontSector = m_bsp->m_gameLevel->sectors[segment.frontSector];
    auto backSector = m_bsp->m_gameLevel->sectors[segment.backSector];
    auto line = m_bsp->m_gameLevel->linedefs[segment.linedefNumber];
    auto frontside = m_bsp->m_gameLevel->sidedefs[line.frontSidedef];

    auto lightLevel = frontSector.lightLevel;

    // TODO: Determine player height based on current sector
    float worldFrontZ1 = frontSector.ceilingHeight - m_playerHeight;
    float worldBackZ1 = backSector.ceilingHeight - m_playerHeight;
    float worldFrontZ2 = frontSector.floorHeight - m_playerHeight;
    float worldBackZ2 = backSector.floorHeight - m_playerHeight;

    bool drawUpperWall = false;
    bool drawCeil = false;
    if (worldFrontZ1 != worldBackZ1 ||
        frontSector.lightLevel != backSector.lightLevel ||
        strcmp((const char *)frontSector.ceilingTextureName,
               (const char *)backSector.ceilingTextureName) != 0) {
      drawUpperWall =
          strncmp((const char *)frontside.upperTextureName, "-", 1) != 0 &&
          worldBackZ1 < worldFrontZ1;
      drawCeil = worldFrontZ1 >= 0;
    }

    bool drawLowerWall = false;
    bool drawFloor = false;
    if (worldFrontZ2 != worldBackZ2 ||
        frontSector.lightLevel != backSector.lightLevel ||
        strcmp((const char *)frontSector.floorTextureName,
               (const char *)backSector.floorTextureName) != 0) {
      drawLowerWall =
          strncmp((const char *)frontside.lowerTextureName, "-", 1) != 0 &&
          worldBackZ2 > worldFrontZ2;
      drawFloor = worldFrontZ2 <= 0;
    }

    if (!drawUpperWall && !drawCeil && !drawLowerWall && !drawFloor) {
      return;
    }

    auto rwNormalAngle = segment.angle + 90;
    auto offsetAngle = rwNormalAngle - inAngle;

    auto startingVertex =
        m_bsp->m_gameLevel->vertexes[segment.startingVectorNumber];
    float dx = m_playerTransform.positionX - startingVertex.x;
    float dy = m_playerTransform.positionY - startingVertex.y;
    float hypotenuse = sqrt(dx * dx + dy * dy);
    float rwDistance = hypotenuse * cosf(DEG2RAD(offsetAngle));

    float rwScaleStep = 0;
    float rwScale1 = scaleFromGlobalAngle(x1, rwNormalAngle, rwDistance,
                                          RAD2DEG(m_playerTransform.angle));
    if (x2 > x1) {
      float scale2 = scaleFromGlobalAngle(x2, rwNormalAngle, rwDistance,
                                          RAD2DEG(m_playerTransform.angle));
      rwScaleStep = (scale2 - rwScale1) / (x2 - x1);
    }

    float rwOffset = 0;
    float rwCenterAngle = 0;

    bool segmentTextured = drawUpperWall || drawLowerWall;
    if (segmentTextured) {
      rwOffset = hypotenuse * sinf(DEG2RAD(offsetAngle));
      rwOffset += segment.offset + frontside.xOffset;
      rwCenterAngle = rwNormalAngle - RAD2DEG(m_playerTransform.angle);
    }

    float wallY1 = H_HEIGHT - worldFrontZ1 * rwScale1;
    float wallY1Step = -rwScaleStep * worldFrontZ1;
    float wallY2 = H_HEIGHT - worldFrontZ2 * rwScale1;
    float wallY2Step = -rwScaleStep * worldFrontZ2;

    float portalY1 = 0;
    float portalY1Step = 0;
    if (drawUpperWall) {
      if (worldBackZ1 > worldFrontZ2) {
        portalY1 = H_HEIGHT - worldBackZ1 * rwScale1;
        portalY1Step = -rwScaleStep * worldBackZ1;
      } else {
        portalY1 = wallY2;
        portalY1Step = wallY2Step;
      }
    }

    float portalY2 = 0;
    float portalY2Step = 0;
    if (drawLowerWall) {
      if (worldBackZ2 < worldFrontZ1) {
        portalY2 = H_HEIGHT - worldBackZ2 * rwScale1;
        portalY2Step = -rwScaleStep * worldBackZ2;
      } else {
        portalY2 = wallY1;
        portalY2Step = wallY1Step;
      }
    }

    // Note: Wall texture is taken from frontside after swap?
    std::unique_ptr<GameLevelTexture> &ceilGameTexture =
        m_bsp->m_gameLevel->flatImages[std::string(
            (char *)frontSector.ceilingTextureName, 8)];
    std::unique_ptr<GameLevelTexture> &floorGameTexture =
        m_bsp->m_gameLevel
            ->flatImages[std::string((char *)frontSector.floorTextureName, 8)];
    std::unique_ptr<GameLevelTexture> &lowerWallGameTexture =
        m_bsp->m_gameLevel
            ->textureImages[std::string((char *)frontside.lowerTextureName, 8)];
    std::unique_ptr<GameLevelTexture> &upperWallGameTexture =
        m_bsp->m_gameLevel
            ->textureImages[std::string((char *)frontside.upperTextureName, 8)];

    float vTop = 0;
    float upperTextureAlt = 0;
    if (drawUpperWall) {
      if ((line.flags & 0x8) > 0) {
        vTop = backSector.ceilingHeight + upperWallGameTexture->height;
        upperTextureAlt = vTop - m_playerHeight;
      } else {
        upperTextureAlt = worldFrontZ1;
      }
    }

    float lowerTextureAlt = 0;
    if (drawLowerWall) {
      if ((line.flags & 0x10) > 0) {
        lowerTextureAlt = worldBackZ1;
      } else {
        lowerTextureAlt = worldBackZ2;
      }
    }

    for (int x = x1; x < x2 + 1; x++) {
      float drawWallY1 = wallY1 - 1;
      float drawWallY2 = wallY2;

      float angle = 0;
      float textureColumn = 0;
      float invScale = 0;
      if (segmentTextured) {
        angle = rwCenterAngle - m_x2Angle[x];
        textureColumn = rwDistance * tanf(DEG2RAD(angle)) - rwOffset;
        invScale = 1.0 / rwScale1;
      }

      if (drawUpperWall) {
        float drawUpperWallY1 = wallY1 - 1;
        float drawUpperWallY2 = portalY1;
        if (drawCeil) {
          float cy1 = m_upperClip[x] + 1;
          float cy2 = std::min<float>(drawWallY1 - 1, m_lowerClip[x] - 1);
          renderFlat(x, cy1, cy2, worldFrontZ1, lightLevel, ceilGameTexture);
        }

        float wy1 = std::max<float>(drawUpperWallY1, m_upperClip[x] + 1);
        float wy2 = std::min<float>(drawUpperWallY2, m_lowerClip[x] - 1);

        renderWallColumn(x, wy1, wy2, upperTextureAlt, textureColumn, invScale,
                         lightLevel, upperWallGameTexture);

        if (m_upperClip[x] < wy2) {
          m_upperClip[x] = wy2;
        }

        portalY1 += portalY1Step;
      }

      if (drawCeil) {
        float cy1 = m_upperClip[x] + 1;
        float cy2 = std::min<float>(drawWallY1 - 1, m_lowerClip[x] - 1);
        renderFlat(x, cy1, cy2, worldFrontZ1, lightLevel, ceilGameTexture);

        if (m_upperClip[x] < cy2) {
          m_upperClip[x] = cy2;
        }
      }

      if (drawLowerWall) {
        if (drawFloor) {
          float fy1 = std::max<float>(drawWallY2 + 1, m_upperClip[x] + 1);
          float fy2 = m_lowerClip[x] - 1;
          renderFlat(x, fy1, fy2, worldFrontZ2, lightLevel, floorGameTexture);
        }

        float drawLowerWallY1 = portalY2 - 1;
        float drawLowerWallY2 = wallY2;

        float wy1 = std::max<float>(drawLowerWallY1, m_upperClip[x] + 1);
        float wy2 = std::min<float>(drawLowerWallY2, m_lowerClip[x] - 1);

        renderWallColumn(x, wy1, wy2, lowerTextureAlt, textureColumn, invScale,
                         lightLevel, ceilGameTexture);

        if (m_lowerClip[x] > wy1) {
          m_lowerClip[x] = wy1;
        }

        portalY2 += portalY2Step;
      }

      if (drawFloor) {
        float fy1 = std::max<float>(drawWallY2 + 1, m_upperClip[x] + 1);
        float fy2 = m_lowerClip[x] - 1;
        renderFlat(x, fy1, fy2, worldFrontZ2, lightLevel, floorGameTexture);
        if (m_lowerClip[x] > drawWallY2 + 1) {
          m_lowerClip[x] = fy1;
        }
      }

      rwScale1 += rwScaleStep;
      wallY1 += wallY1Step;
      wallY2 += wallY2Step;
    }
  }

  void clipPortalWalls(GameLevelSegment segment, int xStart, int xEnd,
                       float angle) {
    std::set<int> currentWall;
    for (int i = xStart; i < xEnd; i++) {
      currentWall.insert(i);
    }

    std::vector<int> intersection;
    std::set_intersection(m_screenSet.begin(), m_screenSet.end(),
                          currentWall.begin(), currentWall.end(),
                          std::inserter(intersection, intersection.begin()));
    if (intersection.size() == 0) {
      return;
    }

    if (intersection.size() == currentWall.size()) {
      drawPortalWallRange(segment, xStart, xEnd - 1, angle);
    } else {
      std::sort(intersection.begin(), intersection.end());
      int x = intersection[0];
      int xx = intersection[intersection.size() - 1];
      for (int i = 0; i < intersection.size() - 1; i++) {
        int x1 = intersection[i];
        int x2 = intersection[i + 1];
        if (x2 - x1 > 1) {
          drawPortalWallRange(segment, x, x1, angle);
          x = x2;
        }
      }
      drawPortalWallRange(segment, x, xx, angle);
    }
  }

  void clipSolidWalls(GameLevelSegment segment, int xStart, int xEnd,
                      float angle) {
    if (!m_screenSet.empty()) {
      std::set<int> currentWall;
      for (int i = xStart; i < xEnd; i++) {
        currentWall.insert(i);
      }

      std::vector<int> intersection;
      std::set_intersection(m_screenSet.begin(), m_screenSet.end(),
                            currentWall.begin(), currentWall.end(),
                            std::inserter(intersection, intersection.begin()));

      if (intersection.size() == 0) {
        return;
      }

      if (intersection.size() == currentWall.size()) {
        drawSolidWallRange(segment, xStart, xEnd - 1, angle);
      } else {
        std::sort(intersection.begin(), intersection.end());
        int x = intersection[0];
        int xx = intersection[intersection.size() - 1];
        for (int i = 0; i < intersection.size() - 1; i++) {
          int x1 = intersection[i];
          int x2 = intersection[i + 1];
          if (x2 - x1 > 1) {
            drawSolidWallRange(segment, x, x1, angle);
            x = x2;
          }
        }
        drawSolidWallRange(segment, x, xx, angle);
      }

      std::set<int> newScreenSet;
      std::set_difference(m_screenSet.begin(), m_screenSet.end(),
                          intersection.begin(), intersection.end(),
                          std::inserter(newScreenSet, newScreenSet.begin()));
      m_screenSet = newScreenSet;
    } else {
      // TODO: STOP TRAVERSING BSP
    }
  }

  void classifySegment(GameLevelSegment segment, float x1, float x2,
                       float angle) {
    if (x1 == x2) {
      return;
    }

    auto backSectorId = segment.backSector;
    if (backSectorId == -1) {
      clipSolidWalls(segment, x1, x2, angle);
      return;
    }
    auto backsector = m_bsp->m_gameLevel->sectors[backSectorId];
    auto frontsector = m_bsp->m_gameLevel->sectors[segment.frontSector];

    if (frontsector.ceilingHeight != backsector.ceilingHeight ||
        frontsector.floorHeight != backsector.floorHeight) {
      clipPortalWalls(segment, x1, x2, angle);
      return;
    }

    auto segLinedef = m_bsp->m_gameLevel->linedefs[segment.linedefNumber];
    auto segLineDefFrontSidedef =
        m_bsp->m_gameLevel->sidedefs[segLinedef.frontSidedef];

    if (strncmp((const char *)frontsector.ceilingTextureName,
                (const char *)backsector.ceilingTextureName, 8) == 0 &&
        strncmp((const char *)frontsector.floorTextureName,
                (const char *)backsector.floorTextureName, 8) == 0 &&
        frontsector.lightLevel == backsector.lightLevel &&
        strncmp((const char *)segLineDefFrontSidedef.middleTextureName, "-",
                8) == 0) {
      return;
    }

    clipPortalWalls(segment, x1, x2, angle);
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
        float x1 = angle2x(std::get<1>(cullingResults));
        float x2 = angle2x(std::get<2>(cullingResults));

        classifySegment(segment, x1, x2, std::get<3>(cullingResults));
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
};
