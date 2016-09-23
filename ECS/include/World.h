#pragma once

#include <vector>
#include "EntityManager.h"
#include "System.h"

class World
{
public:
    World() {}
    ~World();
    EntityManager           &getEntityManager();
    std::vector<System*>    &getSystems();

    template<typename T>
    void                    addSystem()
    {
        _systems.push_back(new T());
    }

private:
    EntityManager           _entityManager;
    std::vector<System*>    _systems;
};