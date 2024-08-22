#include "../include/core/engine/GameEngine.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char *argv[]) {

  DebugSettings debugSettings;
  debugSettings.displayFov = false;
  debugSettings.displayVisibleNodes = false;
  debugSettings.displayVisibleSegments = false;

  GameEngine::InitSettings initSetting;
  initSetting.windowWidth = WIDTH;
  initSetting.windowHeight = HEIGHT;
  initSetting.windowTitle = "DOOM 2.5D";
  initSetting.isFullscreen = false;
#ifdef _DEBUG
  initSetting.isDebug = true;
#else
  initSetting.isDebug = false;
#endif
  initSetting.difficultyLevel = 1;
  initSetting.initialState = GameEngineState::eGame;
  initSetting.debugSettings = debugSettings;

  GameEngine engine(initSetting);
  engine.run();

  return 0;
}
