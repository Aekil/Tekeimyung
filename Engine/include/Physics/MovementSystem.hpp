#pragma once

#include "System.hpp"
#include "Core/Map.hpp"

class MovementSystem : public System
{
public:
    MovementSystem(Map* map);
    virtual ~MovementSystem() {};
    virtual void    update(EntityManager &em, float elapsedTime);

private:
    Map*            _map;
};
