/*
** @Author : Simon AMBROISE
*/

#include <Game/Scripts/Health.hpp>

bool Health::TakeDamage(int health)
{
    this->health -= health;

    if (this->health <= 0)
    {
        this->Death();
        return (true);
    }

    return (false);
}
