#include "Systems/RenderingSystem.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/GravitySystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/AISystem.hpp"
#include "Systems/ParticleSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Window/Keyboard.hpp"
#include "Core/Components.hh"
#include "Sound/SoundManager.hpp"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "Utils/OverlayDebugWindow.hpp"
#include "Utils/LogDebugWindow.hpp"
#include "EntityDebugWindow.hpp"

#include "GameStates/PlayState.hpp"



PlayState::PlayState(): _windowImgui(true) {}

PlayState::~PlayState() {}

void    PlayState::createTile(const glm::vec3& pos, eArchetype type)
{
    Entity* tile;

    tile = EntityFactory::createEntity(type);
    sPositionComponent* tilePos = tile->getComponent<sPositionComponent>();
    tilePos->value.y = pos.y;
    tilePos->value.x = pos.x;
    tilePos->z = pos.z;
    (*_map)[pos.z][pos.y][pos.x] = tile->id;
}

void    PlayState::createParticlesEmittor(const glm::vec3& pos, eArchetype type)
{
    Entity* ps;
    sPositionComponent* psPos;

    ps = EntityFactory::createEntity(type);
    psPos = ps->getComponent<sPositionComponent>();
    psPos->value.x = pos.x;
    psPos->value.y = pos.y;
    psPos->z = pos.z;
}

bool    PlayState::init()
{
    EntityManager &em = _world.getEntityManager();

    EntityFactory::bindEntityManager(&em);

    _map = new Map(em, 20, 15, 4);

    createParticlesEmittor(glm::vec3(5.5f, 5.5f, 1.0f), eArchetype::EMITTER_FIRE);
    createParticlesEmittor(glm::vec3(8.5f, 5.5f, 1.0f), eArchetype::EMITTER_WATER);
    // Create characters
    createEntity(glm::vec3(9, 5, 1), eArchetype::PLAYER);
    createEntity(glm::vec3(0.5f, 5.5f, 1), eArchetype::ENEMY);

    // Init base map
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            createTile(glm::vec3(x, y, 0), eArchetype::BLOCK_GREEN);
        }
    }


    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 20; x++) {
            createTile(glm::vec3(x, y, 1), eArchetype::BLOCK_BROWN);
        }
    }

    for (int y = 8; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            createTile(glm::vec3(x, y, 1), eArchetype::BLOCK_BROWN);
        }
    }


    // Create towers
    createTile(glm::vec3(7, 4, 1), eArchetype::TOWER_FIRE);
    createTile(glm::vec3(7, 7, 1), eArchetype::TOWER_FIRE);

    _world.addSystem<InputSystem>();
    _world.addSystem<AISystem>();
    _world.addSystem<MovementSystem>(_map);
    _world.addSystem<CollisionSystem>(_map);
    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(_map, dynamic_cast<ParticleSystem*>(_world.getSystems()[4])->getEmitters());

    addDebugWindow<OverlayDebugWindow>();
    addDebugWindow<EntityDebugWindow>(glm::vec2(0, 80), glm::vec2(450, 350));
    addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(450, 80), glm::vec2(300, 200));

    // Play sound
    static int idSoundBkgdMusic = SoundManager::getInstance()->registerSound("ressources/sounds/Kalimba.mp3", BACKGROUND_SOUND);
    SoundManager::getInstance()->playSound(idSoundBkgdMusic);

    return (true);
}

Entity*    PlayState::createEntity(const glm::vec3& pos, eArchetype type)
{
    Entity* entity = EntityFactory::createEntity(type);

    sPositionComponent* posEntity = entity->getComponent<sPositionComponent>();
    posEntity->value.x = pos.x;
    posEntity->value.y = pos.y;
    posEntity->z = pos.z;

    (*_map)[pos.z].addEntity(entity->id);

    static int idSoundSpawn = SoundManager::getInstance()->registerSound("ressources/sounds/spawn.mp3", DEFAULT_SOUND);
    SoundManager::getInstance()->playSound(idSoundSpawn);

    return entity;
}

bool    PlayState::update(float elapsedTime)
{
    return (GameState::update(elapsedTime));
}
