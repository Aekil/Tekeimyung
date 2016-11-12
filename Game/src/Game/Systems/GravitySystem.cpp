#include <Game/Components.hh>

#include <Game/Systems/GravitySystem.hpp>

GravitySystem::GravitySystem()
{
    this->addDependency<sGravityComponent>();
    this->addDependency<sPositionComponent>();
    this->addDependency<sDirectionComponent>();
}

void GravitySystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity) {
        sDirectionComponent* direction = entity->getComponent<sDirectionComponent>();
        sGravityComponent* gravity = entity->getComponent<sGravityComponent>();

        direction->value += gravity->value * elapsedTime;
    });
}
