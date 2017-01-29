/**
* @Author   Guillaume Labey
*/

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/glm.hpp>

#include <Engine/Utils/Exception.hpp>
#include <Engine/Window/Keyboard.hpp>
#include <Engine/Window/HandleFullscreenEvent.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Sound/SoundManager.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Animation.hpp>
#include <Engine/Physics/Collisions.hpp>

#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/ResolutionSystem.hpp>
#include <Engine/Systems/GravitySystem.hpp>
#include <Engine/Systems/CollisionSystem.hpp>
#include <Engine/Systems/ParticleSystem.hpp>
#include <Engine/Systems/ScriptSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Systems/MouseSystem.hpp>
#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Game/Utils/PlayStates.hpp>
#include <Game/GameStates/PauseState.hpp>

#include <Game/GameStates/PlayState.hpp>


PlayState::~PlayState() {}

void    PlayState::onEnter() {}

void    PlayState::setupSystems()
{
    _world.addSystem<ScriptSystem>();
    _world.addSystem<MouseSystem>();
    _world.addSystem<CollisionSystem>();
    _world.addSystem<ResolutionSystem>();
    _world.addSystem<ParticleSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<RenderingSystem>(&_camera, _world.getSystem<ParticleSystem>()->getEmitters());
}

bool    PlayState::init()
{
    EntityManager* em = _world.getEntityManager();

    _map = new Map(*em, 20, 15, 4);

    initCamera();
    initEntities();

    _pair = std::make_pair(Keyboard::eKey::F, new HandleFullscreenEvent());

    _backgroundMusic = EventSound::getEventByEventType(eEventSound::BACKGROUND);

    return (true);
}

bool    PlayState::update(float elapsedTime)
{
    auto& gameWindow = GameWindow::getInstance();
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();

    if (keyboard.getStateMap()[_pair.first] == Keyboard::eKeyState::KEY_PRESSED)
        _pair.second->execute();

    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED ||
        gameWindow->hasLostFocus())
    {
        _gameStateManager->addState<PauseState>();
    }

    updateCameraInputs(elapsedTime);

    // Play background music
    #if (ENABLE_SOUND)
        if (_backgroundMusic->soundID != -1 && !SoundManager::getInstance()->isSoundPlaying(_backgroundMusic->soundID))
        {
            SoundManager::getInstance()->playSound(_backgroundMusic->soundID);
        }
    #endif

    return (GameState::update(elapsedTime));
}

void    PlayState::initCamera()
{
    _camera.translate(glm::vec3(350.0f, 250.0f, 300.0f));
    _camera.setDir(glm::vec3(-30.0f));

    // Set camera screen
    float size = 500.0f;
    Camera::sScreen screen;
    screen.right = size * _camera.getAspect();
    screen.left = -screen.right;
    screen.top = size;
    screen.bottom = -screen.top;
    _camera.setScreen(screen);
    _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_3D);
    _camera.setZoom(0.5f);
}

void    PlayState::initEntities()
{
    EntityManager* em = _world.getEntityManager();

    // Create particles emitter
    PlayStates::createTile(eArchetype::EMITTER_WATER, glm::vec3(12, 5, 1));

    // Create character
    Entity* player = PlayStates::createTile(eArchetype::PLAYER, glm::vec3(65, 6.25, 33));

    PlayStates::createTile(eArchetype::GAME_MANAGER, glm::vec3(0, 0, 0));
    PlayStates::createTile(eArchetype::CASTLE);

    // Create spawner
    //static uint32_t spawnerEntityID = SpawnerSystem::createSpawner(_map, glm::vec3(0.5, 5.5f, 0.5), eArchetype::SPAWNER);
    //SpawnerSystem::setNbEntities(*em, spawnerEntityID, 5);
    //SpawnerSystem::setSecBeforeFirstSpawn(*em, spawnerEntityID, 7); // method 1
    //SpawnerSystem::setSecBeforeEachSpawn(*em, spawnerEntityID, 2);
    //SpawnerSystem::setSecBeforeEachSpawn(*em, spawnerEntityID, 5);

    /*static uint32_t spawnerEntityID2 = SpawnerSystem::createSpawner(_map, glm::vec3(0, 8.0f, 1), eArchetype::SPAWNER);
    tSpawnerData spawnerData = { 5, 5, 2, 5 };
    SpawnerSystem::setAllFields(*em, spawnerEntityID2, spawnerData); // method 2

    static uint32_t spawnerEntityID3 = SpawnerSystem::createSpawner(_map, glm::vec3(0, 11.0f, 1), eArchetype::SPAWNER);
    SpawnerSystem::setSecBeforeFirstSpawn(*em, spawnerEntityID3, 1);
    SpawnerSystem::setSecBeforeEachSpawn(*em, spawnerEntityID3, 3); // method 1 (partial)

    static uint32_t spawnerEntityID4 = SpawnerSystem::createSpawner(_map, glm::vec3(0, 2.0f, 1), eArchetype::SPAWNER);
    SpawnerSystem::setAllFields(*em, spawnerEntityID4, *SpawnerSystem::getStructData(3, 1, 5, 2));*/ // method 3 : how to delete the tSpawnerData* in this use ?

    /*static uint32_t spawnerEntityID = SpawnerSystem::createSpawner(_map, glm::vec3(0, 8.0f, 1), eArchetype::SPAWNER);
    tSpawnerData spawnerData = { 5, 3, 2, 3 };
    SpawnerSystem::setAllFields(*em, spawnerEntityID, spawnerData);

    static uint32_t spawnerEntityID2 = SpawnerSystem::createSpawner(_map, glm::vec3(0, 8.0f, 1), eArchetype::SPAWNER);
    tSpawnerData spawnerData2 = { 5, 15, 2, 3 };
    SpawnerSystem::setAllFields(*em, spawnerEntityID2, spawnerData2);*/

    // Create towers
    //PlayStates::createTile(_map, glm::vec3(7, 4, 1), eArchetype::TOWER_FIRE);
    //PlayStates::createTile(_map, glm::vec3(7, 7, 1), eArchetype::TOWER_FIRE);

    // Play first animation of entities
    for (auto entity: em->getEntities())
    {
        sRenderComponent* render = entity.second->getComponent<sRenderComponent>();
        if (render && render->_animator.getAnimationsNb() > 0)
        {
            AnimationPtr currentAnimation = render->_animator.getAnimations()[0];
            render->_animator.play(currentAnimation->getName());
        }
    }
}

void    PlayState::updateCameraInputs(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    auto &&mouse = GameWindow::getInstance()->getMouse();
    auto &&scroll = mouse.getScroll();
    static double lastScrollOffset;

    // Update position
    if (keyboard.isPressed(Keyboard::eKey::D))
        _camera.translate(glm::vec3(60.0f * elapsedTime, 0.0f, -60.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Q))
        _camera.translate(glm::vec3(-60.0f * elapsedTime, 0.0f, 60.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::Z))
        _camera.translate(glm::vec3(-60.0f * elapsedTime, 0.0f, -60.0f * elapsedTime));
    if (keyboard.isPressed(Keyboard::eKey::S))
        _camera.translate(glm::vec3(60.0f * elapsedTime, 0.0f, 60.0f * elapsedTime));

    // Update Projection type
    if (keyboard.isPressed(Keyboard::eKey::O))
        _camera.setProjType(Camera::eProj::ORTHOGRAPHIC_3D);
    else if (keyboard.isPressed(Keyboard::eKey::P))
        _camera.setProjType(Camera::eProj::PERSPECTIVE);

    // Update zoom
    double offset = scroll.yOffset - lastScrollOffset;

    if (offset)
        _camera.zoom((float)(-offset * elapsedTime));
    lastScrollOffset = scroll.yOffset;
}
