/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Enemy.hpp>

void Enemy::start()
{
    this->_render = getComponent<sRenderComponent>();
    this->_render->_animator.play("rotation_enemy", true);
    this->_render->_animator.playQueued("attack", true);
    this->health = 150;
    this->maxHealth = 150;
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_transform, _render);
}

void Enemy::update(float dt)
{
    _transform->translate(glm::vec3(0.0f, 0.0f, -10.0f * dt));

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
