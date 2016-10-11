#include "Graphics/Systems/RenderingSystem.hpp"
#include "Window/Systems/InputSystem.hpp"
#include "Window/Keyboard.hpp"
#include "Physics/MovementSystem.hpp"
#include "Physics/GravitySystem.hpp"
#include "Physics/CollisionSystem.hpp"
#include "Core/Components.hh"
#include "EntityFactory.hpp"

#include "GameStates/PlayState.hpp"


PlayState::PlayState() {}

PlayState::~PlayState() {}

bool    PlayState::init()
{
    EntityManager &em = _world.getEntityManager();
    Entity* player;
    Entity* player2;
    Entity* tile;

    EntityFactory::bindEntityManager(&em);
    player = EntityFactory::createEntity(eArchetype::PLAYER);
    player2 = EntityFactory::createEntity(eArchetype::PLAYER);
    sPositionComponent* posPlayer = player->getComponent<sPositionComponent>();
    sPositionComponent* posPlayer2 = player2->getComponent<sPositionComponent>();
    sInputComponent* input = player2->getComponent<sInputComponent>();
    sHitBoxComponent* hitbox = player2->getComponent<sHitBoxComponent>();

    posPlayer->value.x = 9;
    posPlayer->value.y = 9;
    posPlayer->z = 1;

    posPlayer2->value.x = 7;
    posPlayer2->value.y = 7;
    posPlayer2->z = 1;

    input->moveLeft = Keyboard::eKey::J;
    input->moveRight = Keyboard::eKey::L;
    input->moveUp = Keyboard::eKey::I;
    input->moveDown = Keyboard::eKey::K;

    hitbox->min.y = 320.8;
    hitbox->max.y = 255.9;

    _map = new Map(em, 10, 10, 3);

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            tile = EntityFactory::createEntity(eArchetype::TILE1);
            sPositionComponent* pos = tile->getComponent<sPositionComponent>();
            pos->value.x = x;
            pos->value.y = y;
            pos->z = 0;
            (*_map)[0][y][x] = tile->id;
        }
    }

    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 7; x++) {
            tile = EntityFactory::createEntity(eArchetype::TILE2);
            sPositionComponent* pos = tile->getComponent<sPositionComponent>();
            pos->value.x = x;
            pos->value.y = y;
            pos->z = 1;
            (*_map)[1][y][x] = tile->id;
        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            tile = EntityFactory::createEntity(eArchetype::TILE3);
            sPositionComponent* pos = tile->getComponent<sPositionComponent>();
            pos->value.x = x;
            pos->value.y = y;
            pos->z = 2;
            (*_map)[2][y][x] = tile->id;
        }
    }

    (*_map)[1].addEntity(player->id);
    (*_map)[1].addEntity(player2->id);

    _world.addSystem<InputSystem>();
    //_world.addSystem<GravitySystem>();
    _world.addSystem<MovementSystem>(_map);
    _world.addSystem<CollisionSystem>(_map);
    _world.addSystem<RenderingSystem>(_map);

    return (true);
}
