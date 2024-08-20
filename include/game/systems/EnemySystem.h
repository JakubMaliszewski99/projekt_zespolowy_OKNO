#pragma once
#include"../components/EnemyComponent.h"
#include "../include/core/ecs/ECSManager.h"

class EnemySystem : public System{
public:
    void init(std::shared_ptr<ECSManager> manager){
        m_manager = manager;
    }

    void update(){
        
    }

    bool isEnemy(int type){
        switch(type){
            case eImp:
            case eShotgunGuy:
            case eZombieMan:
                return true;
            default:
                return false;
        }
    }

private:
    std::shared_ptr<ECSManager> m_manager;
};