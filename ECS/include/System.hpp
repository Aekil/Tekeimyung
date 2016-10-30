#pragma once

#include <functional>
#include <typeindex>

#include "EntityManager.hpp"
#include "Component.hh"

class System
{
public:
    System();
    virtual ~System();

    virtual void                        update(EntityManager& em, float elapsedTime) = 0;
    virtual bool                        init();
    void                                forEachEntity(EntityManager& em, std::function<void (Entity* entity)> callback);

    template<typename ComponentType>
    void                                addDependency()
    {
        _components.push_back(std::type_index(typeid(ComponentType)));
    }


protected:
    // Store components type_index (copyable wrapper of type_info)
    std::vector<std::type_index>         _components;
};