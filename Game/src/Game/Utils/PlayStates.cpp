/**
* @Author   Julien Chardon
*/

#include <Engine/Components.hh>

#include <Game/Utils/PlayStates.hpp>


PlayStates::PlayStates()
{}

Entity*    PlayStates::createTile(eArchetype type, const glm::vec3& pos)
{
    Entity* tile;

    tile = EntityFactory::createEntity(type, pos);

    return (tile);
}

Entity*    PlayStates::createTile(eArchetype type)
{
    Entity* tile;

    tile = EntityFactory::createEntity(type);

    return (tile);
}