/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/Components/Components.hh>
#include    <Engine/Physics/Collisions.hpp>
#include    <Engine/EntityFactory.hpp>
#include    <Engine/Physics/Physics.hpp>

#include    <Game/Character/Enemy.hpp>
#include    <Game/Weapons/TeslaWeapon.hpp>

#include    <Game/Weapons/TeslaOrb.hpp>

TeslaOrb::~TeslaOrb()
{
    // Prevent circular dependency crash
    if (_owner != nullptr)
    {
        _owner->setFiredOrb(nullptr);
    }
}

void    TeslaOrb::start()
{
    sRenderComponent*   render = this->getComponent<sRenderComponent>();

    this->_rigidBody = this->getComponent<sRigidBodyComponent>();
    this->_speed = new Attribute(100.0f);
    this->_range = new Attribute(150.0f);

    if (render != nullptr)
        render->_animator.play("spinning");
}

void    TeslaOrb::update(float deltaTime)
{
    if (this->hasReachedItsRange() == true)
    {
        this->triggerExplosion();
        this->Destroy();
    }
}

void    TeslaOrb::onCollisionEnter(Entity* entity)
{
    if (entity->getTag() == "TileBaseTurret")
    {
        LOG_DEBUG("Captain Amari says: ALLEZ NANOBOOST MON GARS SUR.");
        this->triggerNanoboost();
        this->Destroy();
    }
    else if (entity->getTag() == "Enemy")
    {
        this->triggerExplosion();
        this->Destroy();
    }
}

/**
    Called once when the orb is created, it just sets the orb position once.
*/
void    TeslaOrb::setPosition(const glm::vec3& position)
{
    sTransformComponent*    transformComponent = this->getComponent<sTransformComponent>();

    if (transformComponent != nullptr)
    {
        transformComponent->setPos(position);
        this->_startPosition = position;
    }
}

/**
    Called once when the orb is created, it just sets the orb direction once.
*/
void    TeslaOrb::setDirection(const glm::vec3& direction)
{
    this->_rigidBody->velocity = direction * (float) this->_speed->getFinalValue();
}

void    TeslaOrb::setOwner(TeslaWeapon* owner)
{
    _owner = owner;
}

bool    TeslaOrb::hasReachedItsRange()
{
    sTransformComponent*    transform = this->getComponent<sTransformComponent>();

    if (transform != nullptr)
    {
        glm::vec3   currentPosition;
        float       distance;

        currentPosition = transform->getPos();
        distance = Physics::distance(this->_startPosition, currentPosition);
        return (distance >= this->_range->getFinalValue());
    }
    else
        LOG_WARN("Could not retrieve %s from Entity with archetype \"%s\"", "sTransformComponent", "TESLA_ORB");

    return (true);
}

void    TeslaOrb::triggerNanoboost()
{

}

void    TeslaOrb::triggerExplosion()
{
    if (!_owner)
    {
        LOG_ERROR("TeslaOrb don't have owner");
        return;
    }

    sTransformComponent* transform = this->getComponent<sTransformComponent>();
    auto em = EntityFactory::getBindedEntityManager();

    const auto& enemies = em->getEntitiesByTag("Enemy");
    for (Entity* enemy : enemies)
    {
        auto box = enemy->getComponent<sBoxColliderComponent>();
        auto sphere = enemy->getComponent<sSphereColliderComponent>();
        auto enemyTransform = enemy->getComponent<sTransformComponent>();
        bool inRange = false;
        float radius = 6.0f;

        if (box != nullptr)
        {
            if (Collisions::sphereVSAABB(
                transform->getPos(),
                radius * (SIZE_UNIT / 2.0f),
                box->pos + enemyTransform->getPos(),
                glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                inRange = true;
            }
        }
        else if (sphere != nullptr)
        {
            if (Collisions::sphereVSsphere(
                transform->getPos(),
                radius * (SIZE_UNIT / 2.0f),
                sphere->pos + enemyTransform->getPos(),
                sphere->radius * std::max({ enemyTransform->getScale().x, enemyTransform->getScale().y, enemyTransform->getScale().z }) * (SIZE_UNIT / 2.0f)))
            {
                inRange = true;
            }
        }

        if (inRange)
        {
            Enemy* enemyScript = this->getEntityScript<Enemy>(enemy, "Enemy");
            if (!enemyScript)
            {
                LOG_WARN("Could not find script Enemy on entity Enemy");
                continue;
            }

            _owner->triggerLightningEffect(enemy);
            enemyScript->takeDamage(_owner->_attributes["Damage"]->getFinalValue());
        }
    }
}
