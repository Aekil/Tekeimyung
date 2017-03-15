/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Teleport.hpp>

void Teleport::start()
{
}

void Teleport::update(float dt)
{
}

void Teleport::onCollisionEnter(Entity* entity)
{
    if (entity->getTag() == "Player" && this->_enable)
    {
        auto transform = entity->getComponent<sTransformComponent>();

        transform->setPos(this->_tpPos);
    }
}

void Teleport::onCollisionExit(Entity* entity)
{
    this->_enable = !this->_enable;
}

void Teleport::setActivity(bool enable)
{
    this->_enable = enable;
}

void Teleport::setTPPos(glm::vec3 pos)
{
    this->_tpPos = pos;
}