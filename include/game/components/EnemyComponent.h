#pragma once

enum EnemyType{
    eImp = 3004, eShotgunGuy = 3001, eZombieMan = 9
};

enum State{
    Idle, Patrol, Chase, Attack
};

struct EnemyComponent{
    EnemyType type;
    State state;
    bool isAlive;
};