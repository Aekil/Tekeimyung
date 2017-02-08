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
}

void Projectile::Update(float dt)
{
    if (!_targetId)
    {
        return;
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
