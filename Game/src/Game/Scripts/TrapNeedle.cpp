/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/TrapNeedle.hpp>

void TrapNeedle::Start()
{
    this->damage = 35;

    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->_animator.play("scale animation", true);
}

void TrapNeedle::Update(float dt)
{

}