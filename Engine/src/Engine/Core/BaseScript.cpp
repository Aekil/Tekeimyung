/**
* @Author   Simon AMBROISE
*/

#include <Engine/Core/BaseScript.hpp>

#include <Engine/EntityFactory.hpp>

BaseScript::BaseScript()
{
}

void BaseScript::SetEntity(Entity* entity)
{
    this->entity = entity;
}

void BaseScript::Destroy()
{
    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(this->entity);
}

void BaseScript::Instantiate(std::string type, glm::vec3 pos)
{
    EntityFactory::createEntity(type, pos);
}