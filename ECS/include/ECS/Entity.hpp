/**
* @Author   Guillaume Labey
*/

#pragma once

#include <vector>
#include <cstdint>
#include <functional>

#include <ECS/Component.hh>

#define POWER_TWO(x) (1 << (x))

#define ENTITY_HANDLE_INDEX_BITS 20
#define ENTITY_HANDLE_COUNT_BITS 12
#define ENTITY_HANDLE_MAX_COUNT POWER_TWO(ENTITY_HANDLE_COUNT_BITS)

class EntityManager;
class EntityPool;

class Entity
{
friend EntityManager;
friend EntityPool;

public:
    struct sHandle
    {
        sHandle();
        sHandle(uint32_t handle);
        sHandle(const sHandle& handle);

        sHandle& operator=(const sHandle& handle);

        union
        {
            struct
            {
                uint32_t index: ENTITY_HANDLE_INDEX_BITS;
                uint32_t count: ENTITY_HANDLE_COUNT_BITS;
            };

            uint32_t value;
        };

        explicit operator uint32_t();
        bool operator!();
        bool operator!=(const sHandle& handle);
        bool operator==(const Entity::sHandle& rhs) const;
    };

private:
    Entity() {}
    Entity(EntityManager* em, uint32_t handle_): handle(handle_), _em(em) {}

public:
    ~Entity();

    bool operator==(Entity &entity);

    void                            addComponent(sComponent* component);

    template<typename componentType, typename... Args>
    void                            addComponent(Args... args)
    {
        sComponent* component = new componentType(args...);
        _components.push_back(component);
        _em->notifyEntityNewComponent(this, component);
    }

    template<typename componentType>
    componentType*                  getComponent() const
    {
        return static_cast<componentType*>(getComponent(componentType::identifier));
    }

    sComponent*                     getComponent(size_t componentHashCode) const;
    std::vector<sComponent*>&       getComponents();
    const std::vector<sComponent*>& getComponents() const;

    void                            removeComponent(sComponent* component);

    // Check if the entity has the component with corresponding hashcode
    bool                            hasComponent(size_t componentHashCode) const;

    template<typename componentType>
    bool                            hasComponent() const
    {
        return (hasComponent(componentType::identifier));
    }

    void                            setTag(const std::string& tag);
    const std::string&              getTag() const;

public:
    sHandle                         handle;
    std::vector<sComponent*>        _components;
    bool                            _free;

private:
    std::string                     _tag;
    EntityManager*                  _em;
};


// Add hash method for Entity::sHandler so it can be used in a std::map
namespace std {
    template <> struct hash<Entity::sHandle>
    {
        size_t operator()(const Entity::sHandle& handle) const
        {
            return (hash<uint32_t>()(handle.value));
        }
    };
}
