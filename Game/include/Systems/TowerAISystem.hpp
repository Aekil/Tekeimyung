#pragma once

#include <Core/Map.hpp>
#include <System.hpp>

class   TowerAISystem : public System
{
public:
    TowerAISystem(Map* map);
    virtual     ~TowerAISystem();
    virtual void update(EntityManager &em, float elapsedTime);
private:
    Map*    _map;
};