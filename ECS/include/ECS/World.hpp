#pragma once

#include <vector>
#include <memory>

#include <ECS/EntityManager.hpp>
#include <ECS/System.hpp>

class World
{
public:
    World();
    ~World();

    EntityManager*          getEntityManager();
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

    void                        notifyEntityNewComponent(Entity* entity, sComponent* component);
    void                        notifyEntityRemovedComponent(Entity* entity, sComponent* component);
    void                        notifyEntityDeleted(Entity* entity);

private:
    std::shared_ptr<EntityManager>  _entityManager;
    std::vector<System*>    _systems;
};
