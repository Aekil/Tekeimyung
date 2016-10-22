#include "Window/GameWindow.hpp"
#include "Core/Components.hh"

#include "Systems/AISystem.hpp"

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

        direction->value.x = 1.0;
        direction->orientation = eOrientation::SE;
        direction->moved = true;
    });
}
