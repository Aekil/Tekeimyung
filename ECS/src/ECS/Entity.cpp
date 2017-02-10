/**
* @Author   Guillaume Labey
*/

#include <ECS/EntityManager.hpp>

#include <ECS/Entity.hpp>

bool Entity::operator==(uint32_t id_)
{
    return (id == id_);
}

bool Entity::operator==(Entity &entity)
{
    return (id == entity.id);
}

void    Entity::addComponent(sComponent* component)
{
    _components.push_back(component);
    _em->notifyEntityNewComponent(this, component);
}

sComponent* Entity::getComponent(size_t componentHashCode) const
{
    for (const auto& component: _components)
    {
        if (component->id == componentHashCode)
        {
            return (component);
        }
    }

    return (nullptr);
}

std::vector<sComponent*>&   Entity::getComponents()
{
    return _components;
}

const std::vector<sComponent*>&   Entity::getComponents() const
{
    return _components;
}

void    Entity::removeComponent(sComponent* component)
{
    for (auto it = _components.begin(); it != _components.end(); ++it)
    {
        if (*it == component)
        {
            _em->notifyEntityRemovedComponent(this, component);
            delete *it;
            _components.erase(it);
            return;
        }
    }
}

bool    Entity::hasComponent(size_t componentHashCode) const
{
    for (const auto& component: _components)
    {
        if (component->id == componentHashCode)
        {
            return (true);
        }
    }

    return (false);
}

void    Entity::setTag(const std::string& tag)
{
    _tag = tag;
}

const std::string&  Entity::getTag() const
{
    return _tag;
}
