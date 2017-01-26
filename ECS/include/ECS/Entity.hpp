/**
* @Author   Guillaume Labey
*/

#pragma once

#include <vector>
#include <cstdint>

#include <ECS/Component.hh>

class EntityManager;

class Entity
{
public:
    Entity(EntityManager* em, uint32_t id_): id(id_), _em(em) {}
    ~Entity() {}

    bool operator==(uint32_t id_);
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

public:
    uint32_t                        id;
    std::vector<sComponent*>        _components;

private:
    EntityManager*                  _em;
};
