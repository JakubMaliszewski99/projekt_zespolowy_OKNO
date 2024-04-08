#include "../include/Enemy.h"
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <ctime>

Enemy::Enemy()
{
    std::clog << "Utworzenie obiektu enemy" << std::endl;

    positionX = 0.0;
    positionY = 0.0;
    velocity = 0.0;
    angle = 0.0;
}

Enemy::Enemy(int windowSizeX, int windowSizeY, CollisionSolver* newSolver): Actor(newSolver)
{
    std::clog << "Utworzenie obiektu enemy" << std::endl;

    std::srand(std::time(nullptr));
    positionX = std::rand()%windowSizeX;
    positionY = std::rand()%windowSizeY;
    velocity = 0.0;
    angle = 0.0;
}

Enemy::~Enemy()
{

}