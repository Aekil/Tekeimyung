#pragma once

#include <iostream>
#include <vector>
#include <typeinfo>
#include "Component.hh"

class Entity
{
public:
    Entity(unsigned int id_): id(id_) {}
    ~Entity() {}

    bool operator==(unsigned int id_)
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
        Component* component = new componentType(args...);
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
    unsigned int                    id;
    std::vector<Component*>         _components;
};