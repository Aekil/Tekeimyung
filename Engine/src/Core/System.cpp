#include <algorithm>
#include "Core/System.hpp"

System::System() {}

System::~System() {}

bool                System::init()
{
    return true;
}

void                System::forEachEntity(EntityManager& em, std::function<void (Entity* entity)> callback)
{
    for (auto &&entity: em.getEntities())
    {
        bool hasComponents = true;

        for (std::type_index componentType: _components)
        {
            size_t hashCode = componentType.hash_code();
            if (!entity->hasComponent(hashCode))
            {
                hasComponents = false;
                break;
            }
        }

        if (hasComponents)
            callback(entity);
    }
}