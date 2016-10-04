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

    _world.addSystem<InputSystem>();
    _world.addSystem<GravitySystem>();
    _world.addSystem<MovementSystem>();
    _world.addSystem<RenderingSystem>();

    Entity *entity = em.createEntity();
    entity->addComponent<sPositionComponent>(9, 9, 3);
    entity->addComponent<sDirectionComponent>(0, 0);
    entity->addComponent<sGravityComponent>(glm::vec2(9.8, 9.8));
    entity->addComponent<sInputComponent>(Keyboard::eKey::Q, Keyboard::eKey::D, Keyboard::eKey::Z, Keyboard::eKey::S);
    entity->addComponent<sRenderComponent>(Sprite::eType::OBJECT, "ressources/sprites/soldierSpriteSheet.png", true, 8,
        std::vector<eOrientation>{eOrientation::NW, eOrientation::N, eOrientation::NE, eOrientation::E, eOrientation::W, eOrientation::SW, eOrientation::S, eOrientation::SE}, glm::vec2(55, 64.9));

    _map.resize(3);
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            entity = em.createEntity();
            entity->addComponent<sPositionComponent>(x, y, 0);
            entity->addComponent<sRenderComponent>(Sprite::eType::TILE, "ressources/sprites/Landscape/landscape_13.png");
            _map[0][y][x] = entity;
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            entity = em.createEntity();
            entity->addComponent<sPositionComponent>(x, y, 2);
            entity->addComponent<sRenderComponent>(Sprite::eType::TILE, "ressources/sprites/Landscape/landscape_30.png");
            _map[2][y][x] = entity;
        }
    }
   for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 7; x++) {
            entity = em.createEntity();
            entity->addComponent<sPositionComponent>(x, y, 1);
            entity->addComponent<sRenderComponent>(Sprite::eType::TILE, "ressources/sprites/Landscape/landscape_33.png");
            _map[1][y][x] = entity;
        }
    }

    // Sort entities with their depth value
    em.sortEntities([](Entity *first, Entity *second) {
        unsigned int depthFirst = 0;
        unsigned int depthSecond = 0;
        sPositionComponent *positionFirst = first->getComponent<sPositionComponent>();
        sPositionComponent *positionSecond = second->getComponent<sPositionComponent>();

        if (positionFirst)
        {
            depthFirst = positionFirst->x + positionFirst->y + positionFirst->z;
        }
        if (positionSecond)
        {
            depthSecond = positionSecond->x + positionSecond->y + positionSecond->z;
        }

        return depthFirst < depthSecond;
    });





    return (true);
}
