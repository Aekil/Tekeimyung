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

        for (auto componentHash : _components)
        {
            if (!entity.second->hasComponent(componentHash))
            {
                hasComponents = false;
                break;
            }
        }

        if (hasComponents)
            callback(entity.second);
    }
    em.destroyEntities();
}
