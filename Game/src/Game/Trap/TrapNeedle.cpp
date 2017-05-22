/*
** @Author : Simon AMBROISE
*/

#include <Engine/Core/Components/Components.hh>

#include <Game/Trap/TrapNeedle.hpp>

void TrapNeedle::start()
{
    this->damage = 35;

    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->_animator.play("scale animation", true);
}

void TrapNeedle::update(float dt)
{

}
