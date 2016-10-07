#include "Graphics/Systems/RenderingSystem.hpp"
#include "Window/Systems/InputSystem.hpp"
#include "Window/Keyboard.hpp"
#include "Physics/MovementSystem.hpp"
#include "Physics/GravitySystem.hpp"
#include "Core/Components.hh"

#include "GameStates/PlayState.hpp"


PlayState::PlayState() {}

PlayState::~PlayState() {}

bool    PlayState::init()
{
    EntityManager &em = _world.getEntityManager();
    Entity* player;
    Entity* tile;

    player = em.createEntity();
    player->addComponent<sPositionComponent>(9, 9, 1);
    player->addComponent<sDirectionComponent>(0, 0);
    player->addComponent<sGravityComponent>(glm::vec2(9.8, 9.8));
    player->addComponent<sInputComponent>(Keyboard::eKey::Q, Keyboard::eKey::D, Keyboard::eKey::Z, Keyboard::eKey::S);
    player->addComponent<sRenderComponent>(Sprite::eType::OBJECT, "ressources/sprites/soldierSpriteSheet.png", true, 8,
        std::vector<eOrientation>{eOrientation::NW, eOrientation::N, eOrientation::NE, eOrientation::E, eOrientation::W, eOrientation::SW, eOrientation::S, eOrientation::SE}, glm::vec2(55, 64.9));
    player->addComponent<sTypeComponent>(eEntityType::CHARACTER);


    _map = new Map(em, 10, 10, 3);

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            tile = em.createEntity();
            tile->addComponent<sPositionComponent>(x, y, 0);
            tile->addComponent<sRenderComponent>(Sprite::eType::TILE, "ressources/sprites/Landscape/landscape_13.png");
            tile->addComponent<sTypeComponent>(eEntityType::TILE);
            (*_map)[0][y][x] = tile->id;
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            tile = em.createEntity();
            tile->addComponent<sPositionComponent>(x, y, 2);
            tile->addComponent<sRenderComponent>(Sprite::eType::TILE, "ressources/sprites/Landscape/landscape_30.png");
            tile->addComponent<sTypeComponent>(eEntityType::TILE);
            (*_map)[2][y][x] = tile->id;
        }
    }
   for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 7; x++) {
            tile = em.createEntity();
            tile->addComponent<sPositionComponent>(x, y, 1);
            tile->addComponent<sRenderComponent>(Sprite::eType::TILE, "ressources/sprites/Landscape/landscape_33.png");
            tile->addComponent<sTypeComponent>(eEntityType::TILE);
            (*_map)[1][y][x] = tile->id;
        }
    }

    (*_map)[1][9][9] = player->id;

    _world.addSystem<InputSystem>();
    _world.addSystem<GravitySystem>();
    _world.addSystem<MovementSystem>(_map);
    _world.addSystem<RenderingSystem>(_map);

    return (true);
}
