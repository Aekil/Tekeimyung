/*
** @Author : Guillaume Labey
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/Projectile.hpp>

void Projectile::Start()
{
    _targetId = 0;
    _projectileTransform = entity->getComponent<sTransformComponent>();
    _projectileCollider = entity->getComponent<sSphereColliderComponent>();
    _projectileRigidBody = entity->getComponent<sRigidBodyComponent>();
    _projectileEmitter = entity->getComponent<sParticleEmitterComponent>();
    _speed = 150.0f;
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
        sScriptComponent* script = entity->getComponent<sScriptComponent>();
        if (!script)
        {
            destroyProjectile();
            LOG_WARN("Target has no Enemy tag");
            return;
        }

        Enemy* enemy = script->getScript<Enemy>("Enemy");
        if (enemy->TakeDamage(_damage))
        {
            EntityManager* em = EntityFactory::getBindedEntityManager();
            Entity* target = entity;

            Entity* explosion = Instantiate("ENEMY_EXPLOSION");
            sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
            sTransformComponent* targetTransform = target->getComponent<sTransformComponent>();
            explosionTransform->pos = targetTransform->pos;
            explosionTransform->needUpdate = true;
            _targetId = 0;
        }
    }
    destroyProjectile();
}

void Projectile::followTarget(Entity* target)
{
    sTransformComponent* targetTransform = target->getComponent<sTransformComponent>();

    _projectileRigidBody->velocity = glm::normalize(targetTransform->pos - (_projectileTransform->pos + _projectileCollider->pos)) * _speed;
}

void Projectile::followDirection(const glm::vec3& dir)
{
    _projectileRigidBody->velocity = dir * _speed;
}

void Projectile::destroyProjectile()
{
    // Set a low emitter life instead of destroying the entity
    _projectileEmitter->emitterLife = 0.001f;
}
