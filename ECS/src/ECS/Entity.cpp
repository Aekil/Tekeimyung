/**
* @Author   Guillaume Labey
*/

#include <algorithm>

#include <ECS/EntityManager.hpp>

#include <ECS/Entity.hpp>

Entity::sHandle::sHandle(): value(0) {}

Entity::sHandle::sHandle(uint32_t handle): value(handle) {}

Entity::sHandle::sHandle(const sHandle& handle): value(handle.value) {}

Entity::sHandle& Entity::sHandle::operator=(const sHandle& handle)
{
    value = handle.value;
    return (*this);
}

Entity::sHandle::operator   uint32_t()
{
    return (value);
}

bool    Entity::sHandle::operator!()
{
    return (!value);
}

bool    Entity::sHandle::operator!=(const sHandle& handle)
{
    return (value != handle.value);
}

bool    Entity::sHandle::operator==(const Entity::sHandle& rhs) const
{
    return (value == rhs.value);
}

Entity::~Entity()
{
    std::for_each(_components.begin(), _components.end(), [this](sComponent* component)
    {
        delete component;
    });
    _components.clear();
}

bool Entity::operator==(Entity &entity)
{
    return (handle == entity.handle);
}

void    Entity::addComponent(sComponent* component)
{
    component->entity = this;
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
    _em->removeEntityFromTagGroup(this, _tag);
    _tag = tag;
    _em->addEntityToTagGroup(this, _tag);
}

const std::string&  Entity::getTag() const
{
    return _tag;
}
