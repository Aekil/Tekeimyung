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

void BaseScript::Destroy()
{
    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(this->entity);
}

void BaseScript::Destroy(Entity* entity)
{
    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(entity);
}

const std::vector<Entity*>& BaseScript::GetEntitiesByTag(const std::string& tag);
{
    auto em = EntityFactory::getBindedEntityManager();

    return em->getEntitiesByTag(tag);
}

Entity* BaseScript::Instantiate(std::string type, glm::vec3 pos)
{
    return EntityFactory::createEntity(type, pos);
}
