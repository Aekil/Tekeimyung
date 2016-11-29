#pragma once

#include <vector>

#include <ECS/EntityManager.hpp>
#include <ECS/System.hpp>

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

    template<typename T>
    T*                      getSystem()
    {
        uint32_t id = T::identifier;

        for (auto system_: _systems)
        {
            if (system_->getId() == id)
            {
                return (static_cast<T*>(system_));
            }
        }

        return (nullptr);
    }

private:
    EntityManager           _entityManager;
    std::vector<System*>    _systems;
};
