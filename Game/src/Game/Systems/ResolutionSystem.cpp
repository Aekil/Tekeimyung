#include <Game/Components.hh>

#include <Game/Systems/ResolutionSystem.hpp>
#include <Game/Utils/Resolutions.hpp>

ResolutionSystem::ResolutionSystem()
{
    addDependency<sResolutionComponent>();
}

ResolutionSystem::~ResolutionSystem() { }

void ResolutionSystem::update(EntityManager &em, float elapsedTime)
{
    this->forEachEntity(em, [&](Entity* entity)
    {
        sResolutionComponent* resolutionComponent = entity->getComponent<sResolutionComponent>();

        if (resolutionComponent->collidingState == eCollisionState::ENTERING_COLLISION)
        {
            resolutionComponent->onCollisionEnter(resolutionComponent->entityId);
            resolutionComponent->collidingState = eCollisionState::IS_COLLIDING;
        }
        else if (resolutionComponent->collidingState == eCollisionState::EXIT_COLLISION)
        {
            resolutionComponent->onCollisionExit(resolutionComponent->entityId);
            resolutionComponent->collidingState = eCollisionState::NO_COLLISION;
        }
    });
}