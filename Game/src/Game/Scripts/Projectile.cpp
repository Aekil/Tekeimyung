/*
** @Author : Guillaume Labey
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Game/Scripts/Projectile.hpp>

void Projectile::Start()
{
    _targetId = 0;
    _projectileTransform = entity->getComponent<sTransformComponent>();
    _projectileCollider = entity->getComponent<sSphereColliderComponent>();
    _projectileRigidBody = entity->getComponent<sRigidBodyComponent>();
    _projectileEmitter = entity->getComponent<sParticleEmitterComponent>();
}

void Projectile::Update(float dt)
{
    if (!_targetId)
    {
        return;
    }

    EntityManager* em = EntityFactory::getBindedEntityManager();
    Entity* target = em->getEntity(_targetId);
    if (!target)
    {
        LOG_INFO("No target");
        _targetId = 0;
        destroyProjectile();
    }
    else
    {
        followTarget(target);
    }
}

void Projectile::OnCollisionEnter(Entity* entity)
{
    if (entity->id == _targetId || entity->getTag() == "Enemy")
    {
        LOG_INFO("Destroy");
        EntityManager* em = EntityFactory::getBindedEntityManager();
        Entity* target = entity;
        destroyProjectile();
        em->destroyEntityRegister(target);

        Entity* explosion = Instantiate("ENEMY_EXPLOSION");
        sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
        sTransformComponent* targetTransform = target->getComponent<sTransformComponent>();
        explosionTransform->pos = targetTransform->pos;
        explosionTransform->needUpdate = true;
        _targetId = 0;
    }
    else if (entity->getTag() == "BlockBrown")
    {
        destroyProjectile();
    }
}

void Projectile::followTarget(Entity* target)
{
    sTransformComponent* targetTransform = target->getComponent<sTransformComponent>();

    _projectileRigidBody->velocity = glm::normalize(targetTransform->pos - (_projectileTransform->pos + _projectileCollider->pos)) * 80.0f;
}

void Projectile::followDirection(const glm::vec3& dir)
{
    _projectileRigidBody->velocity = dir * 80.0f;
}

void Projectile::destroyProjectile()
{
    // Set a low emitter life instead of destroying the entity
    _projectileEmitter->emitterLife = 0.001f;
}
