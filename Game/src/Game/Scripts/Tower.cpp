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
    _towerRender = entity->getComponent<sRenderComponent>();
    _range = 8.0f;
    _damage = 75;
    _towershootSound = EventSound::getEventByEventType(eEventSound::TOWER_SHOOT);
}

void Tower::update(float dt)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    _lastShotTime += dt;

    if (!_targetId && _lastShotTime >= _fireRate)
    {
        Entity* enemy = getClosestEnemy();
        if (enemy)
        {
            shootTarget(enemy);
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
    auto& pos = entity->getComponent<sTransformComponent>()->getPos();
    auto& towerPos = this->entity->getComponent<sTransformComponent>()->getPos();

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

    projectileScript->_projectileTransform->setPos(_towerTransform->getPos());
    projectileScript->_projectileTransform->translate(glm::vec3(0.0f, _towerRender->getModel()->getMax().y - 20.0f, 0.0f));
    projectileScript->_targetId = target->id;
    projectileScript->_damage = _damage;
    projectileScript->followTarget(target);

#if (ENABLE_SOUND)
    if (_towershootSound->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_towershootSound->soundID))
    {
        SoundManager::getInstance()->playSound(_towershootSound->soundID);
    }
#endif

    _lastShotTime = 0.0f;
}

Entity* Tower::getClosestEnemy()
{
    Entity* closestEnemy = nullptr;
    float closestDistance = 0.0f;
    EntityManager* em = EntityFactory::getBindedEntityManager();
    const auto& enemies = em->getEntitiesByTag("Enemy");

    for (auto &enemy : enemies)
    {
        sTransformComponent* enemyTransform = enemy->getComponent<sTransformComponent>();
        float distance = glm::distance(enemyTransform->getPos(), _towerTransform->getPos());
        if (!closestEnemy ||
            distance < closestDistance)
        {
            closestDistance = distance;
            closestEnemy = enemy;
        }
    }

    if (closestEnemy && isInRange(closestEnemy))
        return (closestEnemy);

    return (nullptr);
}
