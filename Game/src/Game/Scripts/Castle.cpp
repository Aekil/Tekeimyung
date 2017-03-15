#include <Engine/Components.hh>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/Castle.hpp>

void Castle::start()
{
    setHealth(300);
    setMaxHealth(300);

    this->_render = getComponent<sRenderComponent>();
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_render);
}

void Castle::update(float dt)
{
    Health::update(_transform);
}

void Castle::onCollisionEnter(Entity* entity)
{
    if (entity->getComponent<sNameComponent>()->value == "ENEMY")
    {
        //LOG_DEBUG("Damage done");
        this->takeDamage(150);

        sScriptComponent* script = entity->getComponent<sScriptComponent>();
        Enemy* enemy = script ? script->getScript<Enemy>("Enemy") : nullptr;
        enemy->death();
        Entity* explosion = Instantiate("ENEMY_EXPLOSION");
        sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
        sTransformComponent* targetTransform = entity->getComponent<sTransformComponent>();
        explosionTransform->setPos(targetTransform->getPos());
    }
}

void Castle::death()
{
    Entity* explosion = Instantiate("CASTLE_EXPLOSION");
    sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
    explosionTransform->setPos(this->_transform->getPos());

    this->Destroy();
    LOG_DEBUG("GAME OVER");
}