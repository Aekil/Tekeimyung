#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/glm.hpp>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/OverlayDebugWindow.hpp>
#include <Engine/Utils/LogDebugWindow.hpp>
#include <Engine/Utils/MonitoringDebugWindow.hpp>
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
#include <Game/Utils/PlayStates.hpp>

#include <Game/GameStates/PlayState.hpp>


PlayState::PlayState() : _windowImgui(true) {}

PlayState::~PlayState() {}

bool    PlayState::init()
{
    EntityManager &em = _world.getEntityManager();

    EntityFactory::bindEntityManager(&em);

    _map = new Map(em, 20, 15, 4);

    // Create particles emitter
    PlayStates::createParticlesEmittor(glm::vec3(12, 5, 1), eArchetype::EMITTER_WATER);

    // Create character
    PlayStates::createTile(_map, glm::vec3(9, 5, 1), eArchetype::PLAYER);

    // Initialize base map
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            PlayStates::createTile(_map, glm::vec3(x, y, 0), eArchetype::BLOCK_GREEN);
        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 20; x++) {
            PlayStates::createTile(_map, glm::vec3(x, y, 1), eArchetype::BLOCK_BROWN);
        }
    }

    for (int y = 8; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            PlayStates::createTile(_map, glm::vec3(x, y, 1), eArchetype::BLOCK_BROWN);
        }
    }

    // Create wave
    static uint32_t waveEntityID = WaveSystem::createWave(_map, glm::vec3(0.5, 5.5f, 0.5), eArchetype::WAVE_SPAWNER);
    WaveSystem::setNbEntities(em, waveEntityID, 5);
    WaveSystem::setSecBeforeFirstSpawn(em, waveEntityID, 3);
    WaveSystem::setSecBeforeEachSpawn(em, waveEntityID, 2);
    static uint32_t waveEntityID2 = WaveSystem::createWave(_map, glm::vec3(0, 8.0f, 1), eArchetype::WAVE_SPAWNER);
    sWaveComponent wave;
    wave.nbEntities = 5;
    wave.secBeforeFirstSpawn = 5;
    wave.secBeforeEachSpawn = 2;
    WaveSystem::setAllFields(em, waveEntityID2, wave);
    static uint32_t waveEntityID3 = WaveSystem::createWave(_map, glm::vec3(0, 11.0f, 1), eArchetype::WAVE_SPAWNER);
    WaveSystem::setSecBeforeFirstSpawn(em, waveEntityID3, 2);

    // Create towers
    PlayStates::createTile(_map, glm::vec3(7, 4, 1), eArchetype::TOWER_FIRE);
    PlayStates::createTile(_map, glm::vec3(7, 7, 1), eArchetype::TOWER_FIRE);

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
    addDebugWindow<MonitoringDebugWindow>(MonitoringDebugWindow::getInstance());

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
