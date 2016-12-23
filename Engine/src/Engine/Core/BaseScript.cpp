/**
* @Author   Simon AMBROISE
*/

#include <Engine/Core/BaseScript.hpp>

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