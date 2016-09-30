#include "Graphics/Systems/RenderingSystem.hpp"
#include "Core/Components.hh"

#include "GameStates/PlayState.hpp"


PlayState::PlayState() {}

PlayState::~PlayState() {}

bool    PlayState::init()
{
    EntityManager &em = _world.getEntityManager();

    _world.addSystem<RenderingSystem>();

    Entity *entity = em.createEntity();
    entity->addComponent<sPositionComponent>(9, 9, 1);
    entity->addComponent<sDirectionComponent>(0, 0);
    entity->addComponent<sRenderComponent>("ressources/sprites/taxi.png");

    entity = em.createEntity();
    entity->addComponent<sPositionComponent>(8, 8, 1);
    entity->addComponent<sDirectionComponent>(0, 0);
    entity->addComponent<sRenderComponent>("ressources/sprites/police.png");

    return (true);
}