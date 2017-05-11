/**
* @Author   Simon AMBROISE
*/

#include <Engine/Core/BaseScript.hpp>

#include <Engine/EntityFactory.hpp>

BaseScript::BaseScript(): isInitialized(false)
{
}

void BaseScript::setEntity(Entity* entity)
{
    this->entity = entity;
}

Entity* BaseScript::getEntity()
{
    return (entity);
}

void BaseScript::Destroy()
{
    if (!this->entity)
    {
        return;
    }

    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(this->entity->handle);
}

void BaseScript::Destroy(Entity* entity)
{
    if (!entity)
    {
        return;
    }

    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(entity->handle);
}

const std::vector<Entity*>& BaseScript::GetEntitiesByTag(const std::string& tag)
{
    auto em = EntityFactory::getBindedEntityManager();

    return em->getEntitiesByTag(tag);
}

Entity* BaseScript::Instantiate(std::string type, glm::vec3 pos)
{
    return EntityFactory::createEntity(type, pos);
}

const std::string&  BaseScript::getName() const
{
    return (_name);
}

void    BaseScript::setName(const std::string& name)
{
    _name = name;
}
