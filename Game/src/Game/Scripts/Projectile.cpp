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
        em->destroyEntityRegister(this->entity);
    }
    else
    {
        followTarget(target);
    }
}

void Projectile::OnCollisionEnter(Entity* entity)
{
    if (entity->id == _targetId)
    {
        LOG_INFO("Destroy");
        EntityManager* em = EntityFactory::getBindedEntityManager();
        Entity* target = em->getEntity(_targetId);
        em->destroyEntityRegister(this->entity);
        em->destroyEntityRegister(target);
    }
}

void Projectile::followTarget(Entity* target)
{
    sTransformComponent* targetTransform = target->getComponent<sTransformComponent>();

    _projectileRigidBody->velocity = glm::normalize(targetTransform->pos - (_projectileTransform->pos + _projectileCollider->pos)) * 80.0f;
}
