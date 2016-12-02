#pragma once

#include <functional>
#include <cstdint>

#include <ECS/EntityManager.hpp>
#include <ECS/Component.hh>
#include <ECS/crc32.hh>

class System
{
public:
    System(uint32_t id);
    virtual ~System();

    virtual void                        update(EntityManager& em, float elapsedTime) = 0;
    virtual bool                        init();
    void                                forEachEntity(EntityManager& em, std::function<void (Entity* entity)> callback);

    template<typename ComponentType>
    void                                addDependency()
    {
        _components.push_back(ComponentType::identifier);
    }

    uint32_t                            getId() const;

    bool                                hasDependency(sComponent* component) const;
    bool                                entityMatchDependencies(Entity* entity) const;

    virtual void                        onEntityNewComponent(Entity* entity, sComponent* component);
    virtual void                        onEntityRemovedComponent(Entity* entity, sComponent* component);
    virtual void                        onEntityDeleted(Entity* entity);

protected:
    // Store components hashs
    std::vector<uint32_t>           _components;

    std::vector<uint32_t>           _entities;

    uint32_t                        _id;
};


template<typename SystemType>
class BaseSystem: public System
{
    public:\
    BaseSystem(): System(SystemType::identifier) {}
};

#define START_SYSTEM(name) \
    class name : public BaseSystem<name> { \
        public:\
        static constexpr unsigned int identifier = #name##_crc32;

#define END_SYSTEM(name) \
    };
