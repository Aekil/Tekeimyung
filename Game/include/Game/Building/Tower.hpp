/*
** @Author : Guillaume Labey
*/

#pragma once

#include <Engine/Utils/EventSound.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <ECS/Entity.hpp>

class Tower final : public BaseScript
{
public:
    Tower() = default;
    ~Tower() = default;

    void start();
    void update(float dt);

private:
    bool isInRange(Entity* entity);
    void shootTarget(Entity* target);
    Entity* getClosestEnemy();

private:
    Entity::sHandle    _targetHandle;
    float       _fireRate;
    float       _lastShotTime;
    float       _range;
    int         _damage;

    tEventSound* _towershootSound = nullptr;

    sTransformComponent* _towerTransform;
    sRenderComponent* _towerRender;
};

REGISTER_SCRIPT(Tower);
