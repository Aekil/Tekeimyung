#include <Engine/Components.hh>
#include <Engine/BasicState.hpp>
#include <Engine/Utils/LevelLoader.hpp>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/Castle.hpp>

void Castle::start()
{
    setHealth(4200);
    setMaxHealth(4200);

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
    if (entity->getTag() == "Enemy")
    {
        this->takeDamage(200);

        sScriptComponent* script = entity->getComponent<sScriptComponent>();
        Enemy* enemy = script ? script->getScript<Enemy>("Enemy") : nullptr;
        enemy->death();
    }
}

void Castle::death()
{
    //Entity* explosion = Instantiate("CASTLE_EXPLOSION");
    //sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
    //explosionTransform->setPos(this->_transform->getPos());

    this->Destroy();
}
