#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/glm.hpp>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/OverlayDebugWindow.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>
#include <Engine/Window/Keyboard.hpp>
#include <Engine/Window/HandleFullscreenEvent.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Game/Systems/RenderingSystem.hpp>
#include <Game/Systems/ResolutionSystem.hpp>
#include <Game/Systems/MovementSystem.hpp>
#include <Game/Systems/GravitySystem.hpp>
#include <Game/Systems/CollisionSystem.hpp>
#include <Game/Systems/AISystem.hpp>
#include <Game/Systems/ParticleSystem.hpp>
#include <Game/Systems/InputSystem.hpp>
#include <Game/Systems/TowerAISystem.hpp>
#include <Game/Systems/ProjectileSystem.hpp>
#include <Game/Systems/WaveSystem.hpp>
#include <Game/Components.hh>
#include <Game/EntityDebugWindow.hpp>
#include <Game/EntityFactory.hpp>

#include <Game/GameStates/PlayState.hpp>


PlayState::PlayState(): _windowImgui(true) {}

PlayState::~PlayState() {}

void    PlayState::createTile(const glm::vec3& pos, eArchetype type)
{
    Entity* tile;


    if ((int)type == 42) 
    {
        tile = EntityFactory::createEntity(eArchetype::PLAYER);

        sInputComponent *lol = tile->getComponent<sInputComponent>();
        lol->moveUp = Keyboard::eKey::KP_8;
        lol->moveDown = Keyboard::eKey::KP_5;
        lol->moveRight = Keyboard::eKey::KP_6;
        lol->moveLeft = Keyboard::eKey::KP_4;
    } 
    else
    {
        tile = EntityFactory::createEntity(type);
    }

    sPositionComponent* tilePos = tile->getComponent<sPositionComponent>();
    sTransformComponent *tileTransform = tile->getComponent<sTransformComponent>();
    tilePos->value.y = pos.y;
    tilePos->value.x = pos.x;
    tilePos->z = pos.z;
    (*_map)[(uint16_t)pos.z][(uint32_t)pos.y][(uint32_t)pos.x] = tile->id;
    tileTransform->pos = Map::mapToGraphPosition(tilePos->value, tilePos->z);
}

void    PlayState::createWave(const glm::vec3& pos, eArchetype type)
{
    createTile(pos, type);
}

Entity* PlayState::createParticlesEmittor(const glm::vec3& pos, eArchetype type)
{
    Entity* ps;
    sPositionComponent* psPos;
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


void    PlayState::goTo(Entity* emitter, Entity* character)
{
    sPositionComponent* emitterPos;
    sPositionComponent* characterPos;

    emitterPos = emitter->getComponent<sPositionComponent>();
    characterPos = character->getComponent<sPositionComponent>();

    if (!emitterPos || !characterPos)
        EXCEPT(InternalErrorException, "sPositionComponent missing");

    emitter->addComponent<sDirectionComponent>(glm::normalize(characterPos->value - emitterPos->value) * 4.0f);
}

bool    PlayState::init()
{
    EntityManager &em = _world.getEntityManager();

    EntityFactory::bindEntityManager(&em);

    _map = new Map(em, 20, 15, 4);

    // Create particles emitter
    createParticlesEmittor(glm::vec3(12, 5, 1), eArchetype::EMITTER_WATER);

    // Create character
    createTile(glm::vec3(9, 5, 1), eArchetype::PLAYER);

    createTile(glm::vec3(9, 7, 1), (eArchetype)42);

    // Initialize base map
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

    // Create wave
/*    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 1; j++) {
            // create wave
            createWave(glm::vec3(j, 5.0f + i, 0), eArchetype::WAVE_SPAWNER);
        }
    }*/

    // Create towers
    createTile(glm::vec3(7, 4, 1), eArchetype::TOWER_FIRE);
    createTile(glm::vec3(7, 7, 1), eArchetype::TOWER_FIRE);

    _world.addSystem<WaveSystem>(_map);
    _world.addSystem<InputSystem>();
    _world.addSystem<TowerAISystem>(_map);
    _world.addSystem<AISystem>();
    _world.addSystem<ProjectileSystem>();

    _world.addSystem<MovementSystem>(_map);
    _world.addSystem<CollisionSystem>(_map);
    _world.addSystem<ResolutionSystem>();
    _world.addSystem<ParticleSystem>();
    _world.addSystem<RenderingSystem>(_map, _world.getSystem<ParticleSystem>()->getEmitters());

    addDebugWindow<OverlayDebugWindow>();
    addDebugWindow<EntityDebugWindow>(&em, _map, glm::vec2(0, 80), glm::vec2(600, 350));
    addDebugWindow<LogDebugWindow>(Logger::getInstance(), glm::vec2(0, 430), glm::vec2(300, 200));

    // Play sound
/*    static int idSoundBkgdMusic = SoundManager::getInstance()->registerSound("ressources/sounds/Kalimba.mp3", BACKGROUND_SOUND);
    SoundManager::getInstance()->playSound(idSoundBkgdMusic);*/

    _pair = std::make_pair(Keyboard::eKey::F, new HandleFullscreenEvent());
    return (true);
}

bool    PlayState::update(float elapsedTime)
{
    if (GameWindow::getInstance()->getKeyboard().getStateMap()[_pair.first] == Keyboard::eKeyState::KEY_PRESSED)
        _pair.second->execute();
    return (GameState::update(elapsedTime));
}
