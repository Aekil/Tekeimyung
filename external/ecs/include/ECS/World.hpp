/**
* @Author   Guillaume Labey
*/

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

    EntityManager*                          getEntityManager();
    std::vector<std::unique_ptr<System> >&  getSystems();

    template<typename T, typename... Args>
    void                    addSystem(Args... args)
    {
        std::unique_ptr<System> system_ = std::make_unique<T>(args...);
        _systems.push_back(std::move(system_));
    }

    template<typename T>
    T*                      getSystem()
    {
        uint32_t id = T::identifier;

        for (auto& system_: _systems)
        {
            if (system_->getId() == id)
            {
                return (static_cast<T*>(system_.get()));
            }
        }

        return (nullptr);
    }

    void                        notifyEntityNewComponent(Entity* entity, sComponent* component);
    void                        notifyEntityRemovedComponent(Entity* entity, sComponent* component);
    void                        notifyEntityCreated(Entity* entity);
    void                        notifyEntityDeleted(Entity* entity);

private:
    std::unique_ptr<EntityManager>          _entityManager;
    std::vector<std::unique_ptr<System> >    _systems;
};
