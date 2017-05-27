/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/Components/Components.hh>
#include    <Engine/Physics/Physics.hpp>

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

}
