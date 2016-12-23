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
}

void Player::Update(float dt)
{
    sPositionComponent* position = this->getComponent<sPositionComponent>();
    
    //LOG_DEBUG("%f, %f, %f", position->value.x, position->value.y, position->z);
    if (this->m_health > 0)
        this->m_health -= 1;
    //else
        //this->Destroy();
}