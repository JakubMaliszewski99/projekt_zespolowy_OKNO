#include <SFML/Graphics.hpp>
#include "..\include\Game.h"
#include <iostream>

int main(){
    std::clog << "Logi: " << std::endl;

    // Stworzenie obiektu klasy Game
    Game game;

    game.init();

    while (game.isRunning()){
        game.processEvents();

        game.update();

        game.render();
    }

    game.cleanup();

    return 0;
}