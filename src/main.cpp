#include <SFML/Graphics.hpp>
#include <iostream>

class Game{
    public:
        Game(){

        }
        void run(){

        }
    
    private:
        void processEvents();
        void update();
        void render();

    private:
        sf::RenderWindow mWindow;
        sf::CircleShape mPlayer;
};

int main(){
    std::clog << "Logi: " << std::endl;

    Game game;
    game.run();

    return 0;
}