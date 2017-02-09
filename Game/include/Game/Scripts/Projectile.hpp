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
    void followTarget(Entity* target);

private:
    uint32_t _targetId;

    sTransformComponent* _projectileTransform;
    sSphereColliderComponent* _projectileCollider;
    sRigidBodyComponent* _projectileRigidBody;
};

REGISTER_SCRIPT(Projectile);
