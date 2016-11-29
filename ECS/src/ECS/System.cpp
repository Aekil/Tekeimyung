#include <ECS/System.hpp>

System::System(uint32_t id): _id(id) {}

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


uint32_t    System::getId() const
{
    return (_id);
}
