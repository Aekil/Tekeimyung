/**
* @Author   Julien Chardon
*/

#include <Game/Components.hh>

#include <Game/Utils/PlayStates.hpp>


PlayStates::PlayStates()
{}

Entity*    PlayStates::createTile(Map* map, const glm::vec3& pos, eArchetype type)
{
    Entity* tile;


    tile = EntityFactory::createEntity(type, pos);

    (*map)[(uint16_t)pos.z][(uint32_t)pos.y][(uint32_t)pos.x] = tile->id;

    return (tile);
}

Entity* PlayStates::createParticlesEmittor(const glm::vec3& pos, eArchetype type)
{
    Entity* ps;

    ps = EntityFactory::createEntity(type, pos);

    return (ps);
}
