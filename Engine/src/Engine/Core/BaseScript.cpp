/**
* @Author   Simon AMBROISE
*/

#include <Engine/Core/BaseScript.hpp>

BaseScript::BaseScript(/*Entity* entity*/)/* : entity(entity)*/
{
}

void BaseScript::SetEntity(Entity* entity)
{
    this->entity = entity;
}