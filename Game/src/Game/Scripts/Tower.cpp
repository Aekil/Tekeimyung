/*
** @Author : Guillaume Labey
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Game/Scripts/Projectile.hpp>
#include <Game/Scripts/Tower.hpp>

void Tower::Start()
{
    _targetId = 0;
    _fireRate = 2.0f;
    _lastShotTime = _fireRate;
    _towerTransform = entity->getComponent<sTransformComponent>();
    _range = 22.0f;
}

void Tower::Update(float dt)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    _lastShotTime += dt;

    if (!_targetId)
    {
        for (auto &entity : em->getEntities())
        {
            sScriptComponent* scriptComponent = entity.second->getComponent<sScriptComponent>();
            if (scriptComponent &&
                scriptComponent->getScript("Enemy") &&
                isInRange(entity.second) &&
                _lastShotTime >= _fireRate)
            {
                _targetId = entity.second->id;
                shootTarget(entity.second);
                break;
            }
        }
        return;
    }

    Entity* target = em->getEntity(_targetId);

    if (!target || !isInRange(target))
    {
        _targetId = 0;
    }
    else if (_lastShotTime >= _fireRate)
    {
        shootTarget(target);
    }
}

bool Tower::isInRange(Entity* entity)
{
    sBoxColliderComponent* box = entity->getComponent<sBoxColliderComponent>();
    auto& pos = entity->getComponent<sTransformComponent>()->pos;
    auto& towerPos = this->entity->getComponent<sTransformComponent>()->pos;

    if (!box)
        return (false);

    return (Collisions::sphereVSAABB(towerPos, _range * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)));
}

void Tower::shootTarget(Entity* target)
{
    Entity*                 fireball;
    sScriptComponent*       fireballScripts;
    Projectile*             projectileScript;

    fireball = Instantiate("FIRE_BALL");
    fireballScripts = fireball->getComponent<sScriptComponent>();
    projectileScript = fireballScripts->getScript<Projectile>("Projectile");

    projectileScript->_projectileTransform->pos = _towerTransform->pos;
    projectileScript->_projectileTransform->needUpdate = true;
    projectileScript->_targetId = target->id;
    projectileScript->followTarget(target);

    _lastShotTime = 0.0f;
}
