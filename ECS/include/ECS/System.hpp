#pragma once

#include <functional>
#include <cstdint>

#include <ECS/EntityManager.hpp>
#include <ECS/Component.hh>

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
        _components.push_back(ComponentType::identifier);
    }

    virtual const std::type_info& getTypeInfo()
    {
        return (typeid(*this));
    }

protected:
    // Store components hashs
    std::vector<uint32_t>         _components;
};
