cmake_minimum_required(VERSION 3.13)
project(doom)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable (doom 
    src/Actor.cpp
    src/Enemy.cpp
    src/Game.cpp
    src/main.cpp
    src/Menu.cpp
    src/Painter2D.cpp
    src/Player.cpp
)

include_directories (
    "include"
    "libs/ext/SFML/include"
)

add_subdirectory(
    libs/ext/SFML
)

target_link_libraries(doom
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
)