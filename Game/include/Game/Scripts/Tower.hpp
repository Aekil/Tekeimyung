/*
** @Author : Guillaume Labey
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

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
    uint32_t    _targetId;
    float       _fireRate;
    float       _lastShotTime;
    float       _range;
    int         _damage;

    sTransformComponent* _towerTransform;
    sRenderComponent* _towerRender;
};

REGISTER_SCRIPT(Tower);
