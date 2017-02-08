/*
** @Author : Guillaume Labey
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Tower : public BaseScript
{
public:
    Tower() {};
    ~Tower() {};

public:
    void Start();
    void Update(float dt);

private:
    bool isInRange(Entity* entity);
    void shootTarget(Entity* target);

private:
    uint32_t    _targetId;
    float       _fireRate;
    float       _lastShotTime;
    float       _range;

    sTransformComponent* _towerTransform;
};

REGISTER_SCRIPT(Tower);
