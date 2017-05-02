/**
* @Author   Guillaume Labey
*/

#pragma once

#include <vector>
#include <cstdint>
#include <functional>

#include <ECS/Component.hh>

class EntityManager;
class EntityPool;

class Entity
{
friend EntityManager;
friend EntityPool;

public:
    struct sHandle
    {
        sHandle(): value(0) {}
        sHandle(uint32_t handle): value(handle) {}

        union
        {
            struct
            {
                uint32_t index: 20;
                uint32_t count: 12;
            };

            uint32_t value;
        };

        explicit operator uint32_t()
        {
            return (value);
        }

        bool operator!()
        {
            return (!value);
        }

        bool operator!=(const sHandle& handle)
        {
            return (value != handle.value);
        }
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


inline bool operator==(const Entity::sHandle& lhs, const Entity::sHandle& rhs)
{
    return (lhs.value == rhs.value);
}


namespace std {
    template <> struct hash<Entity::sHandle>
    {
        size_t operator()(const Entity::sHandle& handle) const
        {
        return (hash<uint32_t>()(handle.value));
        }
    };
}
