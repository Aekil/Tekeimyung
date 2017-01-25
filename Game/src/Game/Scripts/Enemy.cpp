/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Enemy.hpp>

void Enemy::Start()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->_animator.play("rotation_enemy", true);
}

void Enemy::Update(float dt)
{
}
