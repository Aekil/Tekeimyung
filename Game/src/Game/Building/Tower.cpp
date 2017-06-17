/*
** @Author : Guillaume Labey
*/

#include <Engine/Core/Components/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Physics/Collisions.hpp>
#include <Engine/Debug/Debug.hpp>

#include <Game/Weapons/Projectile.hpp>
#include <Game/Building/Tower.hpp>

REGISTER_SCRIPT(Tower);

void Tower::start()
{
    _targetHandle = 0;
    _fireRate = 1.8f;
    _lastShotTime = _fireRate;
    _towerTransform = entity->getComponent<sTransformComponent>();
    _towerRender = entity->getComponent<sRenderComponent>();
    _range = 12.0f;
    _damage = 125;
    _towershootSound = EventSound::getEventByEventType(eEventSound::TOWER_SHOOT);
}

void Tower::update(float dt)
{
    EntityManager* em = EntityFactory::getBindedEntityManager();
    _lastShotTime += dt;

    if (!_targetHandle && _lastShotTime >= _fireRate)
    {
        Entity* enemy = getClosestEnemy();
        if (enemy)
        {
            shootTarget(enemy);
        }
        return;
    }

    Entity* target = em->getEntity(_targetHandle);

    if (!target || !isInRange(target))
    {
        _targetHandle = 0;
    }
    else if (_lastShotTime >= _fireRate)
    {
        shootTarget(target);
    }
}

bool Tower::isInRange(Entity* entity)
{
    sBoxColliderComponent* box = entity->getComponent<sBoxColliderComponent>();
    sSphereColliderComponent* sphere = entity->getComponent<sSphereColliderComponent>();
    sTransformComponent* transform = entity->getComponent<sTransformComponent>();
    auto& pos = entity->getComponent<sTransformComponent>()->getPos();
    auto& towerPos = this->entity->getComponent<sTransformComponent>()->getPos();

    if (box)
    {
        return (Collisions::sphereVSAABB(
            towerPos,
            _range * (SIZE_UNIT / 2.0f),
            box->pos + pos,
            glm::vec3(SIZE_UNIT * box->size.x * transform->getScale().x,
                SIZE_UNIT * box->size.y * transform->getScale().y,
                SIZE_UNIT * box->size.z * transform->getScale().z)
            ));
    }
    else if (sphere)
    {
        return (Collisions::sphereVSsphere(
            towerPos,
            _range * (SIZE_UNIT / 2.0f),
            sphere->pos + pos,
            sphere->radius * std::max({ transform->getScale().x, transform->getScale().y, transform->getScale().z }) * (SIZE_UNIT / 2.0f)
        ));
    }

    return (false);
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
    projectileScript->_targetHandle = target->handle;
    projectileScript->_damage = _damage;
    projectileScript->followTarget(target);

#if (ENABLE_SOUND)
    if (_towershootSound->soundID != -1)
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
