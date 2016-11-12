#include <ECS/System.hpp>

System::System() {}

System::~System() {}

bool    System::init()
{
    return (true);
}

void    System::forEachEntity(EntityManager& em, std::function<void(Entity* entity)> callback)
{
    for (auto &&entity : em.getEntities())
    {
        bool hasComponents = true;

        for (auto componentType : _components)
        {
            size_t hashCode = componentType.hash_code();
            if (!entity.second->hasComponent(hashCode))
            {
                hasComponents = false;
                break;
            }
        }

        if (hasComponents)
            callback(entity.second);
    }
}
