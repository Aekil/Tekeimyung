/*
** @Author : Simon AMBROISE
*/

#include <Engine/Core/Components/Components.hh>

#include <Game/Trap/TrapFire.hpp>

REGISTER_SCRIPT(TrapFire);

void TrapFire::start()
{
    this->damage = 50;
}

void TrapFire::update(float dt)
{

}
