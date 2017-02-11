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
    this->maxHealth = 150;
    this->_transform = getComponent<sTransformComponent>();
    this->_render = getComponent<sRenderComponent>();
    Health::init(_transform, _render);
}

void Enemy::Update(float dt)
{
    auto pos = &(this->getComponent<sTransformComponent>()->pos);

    pos->z -= 10 * dt;

    Health::update(_transform);
}

void Enemy::Death()
{
    this->Destroy();
}
