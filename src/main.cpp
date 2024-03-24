#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/Game.h"

int main(int argc, char* argv[]){
    std::clog << "Logi: " << std::endl;
    std::clog << "Gra została pomyślnie włączona" << std::endl;

    Game game;
    game.run();

    std::clog << "Gra dobiegła końca" << std::endl;
    return 0;
}