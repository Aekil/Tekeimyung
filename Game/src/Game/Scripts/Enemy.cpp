/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Enemy.hpp>

void Enemy::start()
{
    this->_render = getComponent<sRenderComponent>();
    this->_render->_animator.play("rotation_enemy", true);
    this->health = 150;
    this->maxHealth = 150;
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_transform, _render);
}

void Enemy::update(float dt)
{
    auto pos = &(this->getComponent<sTransformComponent>()->pos);

    pos->z -= 10 * dt;

    Health::update(_transform);
}

void Enemy::death()
{
    this->Destroy();
}

bool Enemy::takeDamage(int damage)
{
    this->_render->_animator.play("hit", false);
    return (Health::takeDamage(damage));
}
