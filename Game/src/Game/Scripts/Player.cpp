/*
** Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Player.hpp>

void Player::Death()
{
    LOG_DEBUG("I'm dead now");
}

void Player::Start()
{
    this->m_health = 150;
    LOG_DEBUG("BORN");
    this->Instantiate(glm::vec3(100, 100, 100));
}

void Player::Update(float dt)
{
    sTransformComponent* position = this->getComponent<sTransformComponent>();

    position->pos.y += 0.5;
    position->needUpdate = true;

    //LOG_DEBUG("%f, %f, %f", position->value.x, position->value.y, position->z);
    if (this->m_health > 0)
        this->m_health -= 1;
    //else
        //this->Destroy();
}