/*
** @Author : Simon AMBROISE
*/

#include <Engine/Core/Components/Components.hh>

#include <Game/Trap/TrapCutter.hpp>

REGISTER_SCRIPT(TrapCutter);

void TrapCutter::start()
{
    this->damage = 60;
}

void TrapCutter::update(float dt)
{

}
