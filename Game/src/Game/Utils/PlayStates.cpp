#include <Game/Components.hh>

#include <Game/Utils/PlayStates.hpp>


PlayStates::PlayStates()
{}

Entity*    PlayStates::createTile(Map* map, const glm::vec3& pos, eArchetype type)
{
    Entity* tile;


    tile = EntityFactory::createEntity(type);

    sPositionComponent *tilePos = tile->getComponent<sPositionComponent>();
    sTransformComponent *tileTransform = tile->getComponent<sTransformComponent>();
    tilePos->value.y = pos.y;
    tilePos->value.x = pos.x;
    tilePos->z = pos.z;
    (*map)[(uint16_t)pos.z][(uint32_t)pos.y][(uint32_t)pos.x] = tile->id;
    tileTransform->pos = Map::mapToGraphPosition(tilePos->value, tilePos->z);

    return (tile);
}

Entity* PlayStates::createParticlesEmittor(const glm::vec3& pos, eArchetype type)
{
    Entity* ps;
    sPositionComponent *psPos;
    sTransformComponent *psTransform;

    ps = EntityFactory::createEntity(type);
    psPos = ps->getComponent<sPositionComponent>();
    psTransform = ps->getComponent<sTransformComponent>();
    psPos->value.x = pos.x;
    psPos->value.y = pos.y;
    psPos->z = pos.z;
    psTransform->pos = Map::mapToGraphPosition(psPos->value, psPos->z);

    return (ps);
}


void    PlayStates::goTo(Entity* emitter, Entity* character)
{
    sPositionComponent* emitterPos;
    sPositionComponent* characterPos;

    emitterPos = emitter->getComponent<sPositionComponent>();
    characterPos = character->getComponent<sPositionComponent>();

    if (!emitterPos || !characterPos)
        EXCEPT(InternalErrorException, "sPositionComponent missing");

    emitter->addComponent<sDirectionComponent>(glm::normalize(characterPos->value - emitterPos->value) * 4.0f);
}
