/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Enemy.hpp>

void Enemy::Start()
{
    auto renderComponent = this->getComponent<sRenderComponent>();

    renderComponent->_animator.play("rotation_enemy", true);
    this->health = 150;
}

void Enemy::Update(float dt)
{
    auto pos = &(this->getComponent<sTransformComponent>()->pos);

    pos->z -= 10 * dt;
}

void Enemy::Death()
{
    this->Destroy();
}