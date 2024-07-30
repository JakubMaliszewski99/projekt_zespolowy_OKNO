cmake_minimum_required(VERSION 3.13)
project(doom)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable (doom 
    src/main.cpp
 "include/core/ecs/Entity.h" 
 "include/core/ecs/ComponentBase.h" 
 "include/core/ecs/EntityManager.h" 
 "src/core/ecs/EntityManager.cpp"   
 "include/core/loaders/WADLoader.h" 
 "include/core/engine/GameLevel.h" 
 "include/core/engine/GameEngine.h" 
 "src/core/engine/GameEngine.cpp" 
 "src/core/loaders/WADLoader.cpp" 
 "src/core/engine/GameLevel.cpp" 
 "include/core/core.h" 
 "include/core/ecs/ComponentArray.h" 
 "include/core/ecs/ComponentManager.h" 
 "include/core/ecs/System.h" 
 "include/core/ecs/SystemManager.h"  
 "include/core/ecs/ECSManager.h" 
 "include/core/logger/Logger.h" 
 "src/core/logger/Logger.cpp" 
 "include/game/components/HealthComponent.h" 
 "include/game/systems/PlayerControllSystem.h" 
 "include/core/engine/InputManager.h" 
 "src/core/engine/InputManager.cpp" 
 "include/game/components/MinimapSpriteComponent.h" 
 "include/game/sprites/PlayerMinimapSprite.h" 
 "src/game/sprites/PlayerMinimapSprite.cpp" 
 "include/game/components/CollectableComponent.h" 
 "include/game/components/SpriteComponent.h" 
 "include/game/sprites/MinimapSprite.h" 
 "include/game/systems/rendering/MinimapRenderingSystem.h" 
 "include/game/components/ControllableComponent.h" 
 "include/game/sprites/LevelMinimapSprite.h" 
 "src/game/sprites/LevelMinimapSprite.cpp" 
 "include/game/sprites/CollectableMinimapSprite.h" 
 "src/game/sprites/CollectableMinimapSprite.cpp" 
 "include/game/systems/CollectableSystem.h" 
 "include/core/engine/BSP.h" 
 "include/core/math/utilities.h" 
 "src/core/math/utilities.cpp" 
 "include/game/systems/rendering/GameRenderingSystem.h" 
 "include/game/components/GameDrawableComponent.h" 
 "include/game/drawable/GameDrawable.h" 
 "include/game/drawable/LevelDrawable.h" 
 "src/game/drawable/LevelDrawable.cpp" 
 "src/core/core.cpp" 
 "include/game/systems/PlayerMovementSystem.h" 
 "include/core/math/collisions.h" 
 "src/core/math/colissions.cpp")

set_target_properties (doom
    PROPERTIES
    CXX_STANDARD 17
)

include_directories (
    "include"
    "libs/ext/SFML/include"
)

add_subdirectory(
    "libs/ext/SFML"
)

target_link_libraries(doom
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
)

file (COPY data DESTINATION ${CMAKE_BINARY_DIR})