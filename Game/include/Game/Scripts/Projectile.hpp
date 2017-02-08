/*
** @Author : Guillaume Labey
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Projectile : public BaseScript
{
friend class Tower;

public:
    Projectile() {};
    ~Projectile() {};

public:
    void Start();
    void Update(float dt);

    void OnCollisionEnter(Entity* entity) override final;

private:
    uint32_t _targetId;

    sTransformComponent* _projectileTransform;
};

REGISTER_SCRIPT(Projectile);
