/**
* @Author   Julien Chardon
*/

#include <Engine/Components.hh>

#include <Game/Utils/PlayStates.hpp>


PlayStates::PlayStates()
{}

Entity*    PlayStates::createTile(Map* map, const glm::vec3& pos, eArchetype type)
{
    Entity* tile;

    if ((int)type == 65) {
        tile = EntityFactory::createEntity(eArchetype::PLAYER, pos);

        auto input = tile->getComponent<sInputComponent>();
        input->moveDown = Keyboard::eKey::KP_5;
        input->moveUp = Keyboard::eKey::KP_8;
        input->moveLeft = Keyboard::eKey::KP_4;
        input->moveRight = Keyboard::eKey::KP_6;
    }
    else {
        tile = EntityFactory::createEntity(type, pos);
    }

    (*map)[(uint16_t)pos.z][(uint32_t)pos.y][(uint32_t)pos.x] = tile->id;

    return (tile);
}

Entity* PlayStates::createParticlesEmittor(const glm::vec3& pos, eArchetype type)
{
    Entity* ps;

    ps = EntityFactory::createEntity(type, pos);

    return (ps);
}
