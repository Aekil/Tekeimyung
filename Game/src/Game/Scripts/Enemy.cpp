/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Enemy.hpp>

void Enemy::start()
{
    this->_render = getComponent<sRenderComponent>();
    this->_render->_animator.play("rotation_enemy", true);
    setHealth(150);
    setMaxHealth(150);
    this->_speed = 50.0f;
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_render);
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

        if (glm::length(direction) > glm::length(targetPos - entityPos))
        {
            _pathProgress++;
        }
    }
    else
    {
        _transform->translate(glm::vec3(0.0f, 0.0f, -this->_speed * dt));
    }

    Health::update(_transform);
}

void    Enemy::onCollisionEnter(Entity* entity)
{
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
