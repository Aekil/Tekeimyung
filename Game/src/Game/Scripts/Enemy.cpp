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
    this->maxHealth = 150;
    this->_speed = 10.0f;
    this->health = this->maxHealth;
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_transform, _render);
}

void Enemy::update(float dt)
{
    if (_path.size() > 0 &&
        _pathProgress < _path.size())
    {
        glm::vec3 targetPos = {_path[_pathProgress].x, this->_transform->getPos().y, _path[_pathProgress].z};
        const glm::vec3& entityPos = this->_transform->getPos();
        glm::vec3 direction = glm::normalize(targetPos - entityPos);

        direction *= this->_speed * dt;
        _transform->translate(direction);

        if (glm::distance(targetPos, entityPos) < 1.0f)
            _pathProgress++;
    }
    else
    {
        _transform->translate(glm::vec3(0.0f, 0.0f, this->_speed * dt));
    }

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

void Enemy::setPath(const std::vector<glm::vec3>& path)
{
    _path = path;
    _pathProgress = 0;
}
