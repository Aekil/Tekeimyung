#pragma once

#include <iostream>
#include <vector>
#include <typeinfo>
#include <cstdint>

#include <ECS/Component.hh>

class Entity
{
public:
    Entity(uint32_t id_): id(id_) {}
    ~Entity() {}

    bool operator==(uint32_t id_)
    {
        return (id == id_);
    }

    bool operator==(Entity &entity)
    {
        return (id == entity.id);
    }

    template<typename componentType, typename... Args>
    void                            addComponent(Args... args)
    {
        sComponent* component = new componentType(args...);
        _components.push_back(component);
    }

    void                            addComponent(sComponent* component)
    {
        _components.push_back(component);
    }

    template<typename componentType>
    componentType*                  getComponent()
    {
        const std::type_info& typeInfo = typeid(componentType);

        for (auto component: _components)
        {
            if (component->getTypeInfo().hash_code() == typeInfo.hash_code())
            {
                return static_cast<componentType*>(component);
            }
        }

        return (nullptr);
    }

    sComponent*                     getComponent(size_t componentHashCode)
    {
        for (auto component: _components)
        {
            if (component->getTypeInfo().hash_code() == componentHashCode)
            {
                return (component);
            }
        }

        return (nullptr);
    }

    // Check if the entity has the component with corresponding hashcode => typeid(component).hash_code()
    bool                            hasComponent(size_t componentHashCode)
    {
        for (auto component: _components)
        {
            if (component->getTypeInfo().hash_code() == componentHashCode)
            {
                return (true);
            }
        }

        return (false);
    }

public:
    uint32_t                        id;
    std::vector<sComponent*>        _components;
};
