#include <Engine/Window/GameWindow.hpp>

#include <Game/Components.hh>

#include <Game/Systems/AISystem.hpp>

AISystem::AISystem()
{
    addDependency<sAIComponent>();
    addDependency<sDirectionComponent>();
}

AISystem::~AISystem() {}

void    AISystem::update(EntityManager &em, float elapsedTime)
{
    forEachEntity(em, [&](Entity *entity) {
        sDirectionComponent *direction = entity->getComponent<sDirectionComponent>();

        direction->value.x = direction->speed;
        direction->moved = true;
    });
}
