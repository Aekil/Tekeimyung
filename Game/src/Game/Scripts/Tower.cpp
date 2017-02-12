/*
** @Author : Guillaume Labey
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Utils/Debug.hpp>

#include <Game/Scripts/Projectile.hpp>
#include <Game/Scripts/Tower.hpp>

void Tower::start()
{
    _targetId = 0;
    _fireRate = 2.5f;
    _lastShotTime = _fireRate;
    _towerTransform = entity->getComponent<sTransformComponent>();
    _range = 22.0f;
    _damage = 50;
}

void Tower::update(float dt)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    _lastShotTime += dt;

    if (!_targetId)
    {
        const auto& enemies = em->getEntitiesByTag("Enemy");
        for (auto &enemy : enemies)
        {
            if (isInRange(enemy) &&
                _lastShotTime >= _fireRate)
            {
                _targetId = enemy->id;
                shootTarget(enemy);
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
    projectileScript->_damage = _damage;
    projectileScript->followTarget(target);

    _lastShotTime = 0.0f;
}
