#pragma once

enum EnemyType{
    eImp = 3004, eShotgunGuy = 3001, eZombieMan = 9
};

struct EnemyComponent{
    EnemyType type;
};