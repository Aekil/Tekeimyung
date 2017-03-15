#include <Engine/Components.hh>

#include <Game/Scripts/Castle.hpp>

void Castle::start()
{
    setHealth(500);
    setMaxHealth(500);
}

void Castle::update(float dt)
{
}

void Castle::onCollisionEnter(Entity* entity)
{
    if (entity->getComponent<sNameComponent>()->value == "PLAYER")
    {
        LOG_DEBUG("Damage done");
        this->takeDamage(25);
    }
}

void Castle::death()
{
    this->Destroy();
    LOG_DEBUG("GAME OVER");
}
