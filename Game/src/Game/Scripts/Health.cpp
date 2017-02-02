/*
** @Author : Simon AMBROISE
*/

#include <Game/Scripts/Health.hpp>

void Health::TakeDamage(int health)
{
    this->health -= health;

    if (this->health <= 0)
        this->Death();
}