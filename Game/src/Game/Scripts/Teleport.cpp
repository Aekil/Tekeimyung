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
    if (this->_enable && this->_init && entity->getTag() == "Player")
    {
        auto transform = entity->getComponent<sTransformComponent>();

        transform->setPos(this->_tpPos);
    }
}

void Teleport::onCollisionExit(Entity* entity)
{
    if (this->_init)
    {
        this->_enable = !this->_enable;
    }
}

void Teleport::setActivity(bool enable)
{
    this->_enable = enable;
}

void Teleport::setTPPos(glm::vec3 pos)
{
    this->_tpPos = pos;
    this->_init = true;
}
