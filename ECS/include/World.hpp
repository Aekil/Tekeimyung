#pragma once

#include <vector>

#include "EntityManager.hpp"
#include "System.hpp"

class World
{
public:
    World();
    ~World();

    EntityManager&          getEntityManager();
    std::vector<System*>&   getSystems();

    template<typename T, typename... Args>
    void                    addSystem(Args... args)
    {
        _systems.push_back(new T(args...));
    }

private:
    EntityManager           _entityManager;
    std::vector<System*>    _systems;
};
