#pragma once
#include <SFML/Graphics.hpp>
#include "core/engine/GameLevel.h"
#include <string>
#include "core/ecs/ECSManager.h"
#include "core/logger/Logger.h"
#include "game/systems/PlayerControllSystem.h"
#include "game/systems/PlayerMovementSystem.h"
#include "game/systems/CollectableSystem.h"
#include "game/systems/rendering/MinimapRenderingSystem.h"
#include "game/systems/rendering/GameRenderingSystem.h"

enum GameEngineState {
  eInitialized, eMainMenu, eInGameMenu, eGame, eGameMinimap
};

// TODO: Do something with InitSettings
class GameEngine {
public:
  struct InitSettings {
    int windowWidth;
    int windowHeight;
    std::string windowTitle;
    bool isFullscreen;
    bool isDebug;
    int difficultyLevel;
    GameEngineState initialState;
    DebugSettings debugSettings;
  };

  GameEngine(InitSettings settings);
  void processEvents();
  void run();
  void init();
  void update(sf::Time deltaTime);

private:
  GameEngineState m_state;
  std::shared_ptr<sf::RenderWindow> m_window;
  InitSettings m_settings;
  std::shared_ptr<ECSManager> m_ecsManager;
  Entity m_playerEntity, m_mapEntity;
  GameLevel *m_level;

  std::shared_ptr<PlayerControllSystem> m_playerControllSystem;
  std::shared_ptr<PlayerMovementSystem> m_playerMovementSystem;
  std::shared_ptr<MinimapRenderingSystem> m_minimapRenderingSystem;
  std::shared_ptr<CollectableSystem> m_collectableSystem;
  std::shared_ptr<GameRenderingSystem> m_gameRenderingSystem;
};