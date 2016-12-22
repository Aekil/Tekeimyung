/*
** Author : Simon AMBROISE
*/

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
    static int deadOnce = 0;

    if (this->m_health > 0)
    {
        LOG_DEBUG("I'm alive");
        this->m_health -= 1;
    }
    else
    {
        if (deadOnce == 0)
        {
            this->Death();
            deadOnce = 1;
        }
    }
}

void Player::OnCollisionEnter(int entityId)
{
    LOG_DEBUG("Collision %d", entityId);
}