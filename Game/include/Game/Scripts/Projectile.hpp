/*
** @Author : Guillaume Labey
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Core/ScriptFactory.hpp>

class Projectile final : public BaseScript
{
friend class Tower;
friend class DefaultWeapon;

public:
    Projectile() = default;
    ~Projectile() = default;

public:
    void start();
    void update(float dt);

    void onCollisionEnter(Entity* entity) override final;

private:
    void followTarget(Entity* target);
    void followDirection(const glm::vec3& dir);
    void destroyProjectile();

private:
    uint32_t _targetId;
    float _speed;
    int _damage;

    sTransformComponent* _projectileTransform;
    sSphereColliderComponent* _projectileCollider;
    sRigidBodyComponent* _projectileRigidBody;
    sParticleEmitterComponent* _projectileEmitter;
};

REGISTER_SCRIPT(Projectile);
