/*
** @Author : Guillaume Labey
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/Projectile.hpp>

void Projectile::start()
{
    _targetId = 0;
    _projectileTransform = entity->getComponent<sTransformComponent>();
    _projectileCollider = entity->getComponent<sSphereColliderComponent>();
    _projectileRigidBody = entity->getComponent<sRigidBodyComponent>();
    _projectileEmitter = entity->getComponent<sParticleEmitterComponent>();
    _speed = 150.0f;
}

void Projectile::update(float dt)
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
        destroyProjectile();
    }
    else
    {
        followTarget(target);
    }
}

void Projectile::onCollisionEnter(Entity* entity)
{
    if (entity->id == _targetId || entity->getTag() == "Enemy")
    {
        sScriptComponent* script = entity->getComponent<sScriptComponent>();
        Enemy* enemy = script ? script->getScript<Enemy>("Enemy") : nullptr;
        if (!script || !enemy)
        {
            destroyProjectile();
            LOG_WARN("Target has no sScriptComponent or no Enemy script");
            return;
        }

        if (enemy->takeDamage(_damage))
        {
            EntityManager* em = EntityFactory::getBindedEntityManager();
            Entity* target = entity;

            Entity* explosion = Instantiate("ENEMY_EXPLOSION");
            sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
            sTransformComponent* targetTransform = target->getComponent<sTransformComponent>();
            explosionTransform->pos = targetTransform->pos;
            explosionTransform->needUpdate();
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
    _targetId = 0;
    // Set a low emitter life instead of destroying the entity
    _projectileEmitter->emitterLife = 0.001f;
    _projectileRigidBody->collisionsEnabled = false;
}
