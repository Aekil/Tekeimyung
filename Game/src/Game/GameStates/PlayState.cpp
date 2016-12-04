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
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Animation.hpp>
#include <Engine/Physics/Collisions.hpp>

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
    EntityManager* em = _world.getEntityManager();

    EntityFactory::bindEntityManager(em);

    _map = new Map(*em, 20, 15, 4);

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

    // Create particles emitter
    PlayStates::createParticlesEmittor(glm::vec3(12, 5, 1), eArchetype::EMITTER_WATER);

    // Create character
    Entity* player = PlayStates::createTile(_map, glm::vec3(9, 5, 1), eArchetype::PLAYER);

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
/*    static uint32_t waveEntityID = WaveSystem::createWave(_map, glm::vec3(0.5, 5.5f, 0.5), eArchetype::WAVE_SPAWNER);
    WaveSystem::setNbEntities(*em, waveEntityID, 5);
    WaveSystem::setSecBeforeFirstSpawn(*em, waveEntityID, 3); // method 1
    WaveSystem::setSecBeforeEachSpawn(*em, waveEntityID, 2);

    static uint32_t waveEntityID2 = WaveSystem::createWave(_map, glm::vec3(0, 8.0f, 1), eArchetype::WAVE_SPAWNER);
    tWaveData waveData = { 5, 5, 2 };
    WaveSystem::setAllFields(*em, waveEntityID2, waveData); // method 2

    static uint32_t waveEntityID3 = WaveSystem::createWave(_map, glm::vec3(0, 11.0f, 1), eArchetype::WAVE_SPAWNER);
    WaveSystem::setSecBeforeFirstSpawn(*em, waveEntityID3, 1);
    WaveSystem::setSecBeforeEachSpawn(*em, waveEntityID3, 3); // method 1 (partial)

    static uint32_t waveEntityID4 = WaveSystem::createWave(_map, glm::vec3(0, 2.0f, 1), eArchetype::WAVE_SPAWNER);
    WaveSystem::setAllFields(*em, waveEntityID4, *WaveSystem::getStructData(3, 1, 5)); // method 3 : how to delete the tWaveData* in this use ?
*/
    // Create towers
    PlayStates::createTile(_map, glm::vec3(7, 4, 1), eArchetype::TOWER_FIRE);
    PlayStates::createTile(_map, glm::vec3(7, 7, 1), eArchetype::TOWER_FIRE);

    addDebugWindow<OverlayDebugWindow>();
    addDebugWindow<EntityDebugWindow>(em, _map, glm::vec2(0, 80), glm::vec2(600, 350));
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

/*    Entity* selectedEntity = getSelectedEntity();
    if (selectedEntity)
    {
        sRenderComponent* render = selectedEntity->getComponent<sRenderComponent>();

        render->color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    }
*/
    return (GameState::update(elapsedTime));
}

Entity* PlayState::getSelectedEntity()
{
    auto camera = Camera::getInstance();
    auto gameWindow = GameWindow::getInstance();
    Mouse& mouse =  gameWindow->getMouse();
    Cursor& cursor = mouse.getCursor();

    glm::vec3 nearScreen(cursor.getX(), gameWindow->getBufferHeight() - cursor.getY(), 0.0f);
    glm::vec3 farScreen(cursor.getX(), gameWindow->getBufferHeight() - cursor.getY(), 1.0f);


    // Unproject 2D points to get 3D points
    // Get 3D point on near plane
    glm::vec3 nearPoint = glm::unProject(nearScreen, camera->getView(), camera->getProj(), gameWindow->getViewport());
    // Get 3D point on far plane
    glm::vec3 farPoint = glm::unProject(farScreen, camera->getView(), camera->getProj(), gameWindow->getViewport());

    EntityManager* em = _world.getEntityManager();
    Entity* selectedEntity = nullptr;
    float selectedEntityDist = 0.0f;

    for (auto it : em->getEntities())
    {
        Entity* entity = it.second;
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // We can't select entity that is not displayed or has model not initialized
        if (!render || !render->_model)
            continue;

        // The selectable zone of an entity is a box
        glm::vec3 boxPos;
        glm::vec3 boxCenter;
        glm::vec3 boxSize;

        // Model box collider position
        boxPos = glm::vec3(render->_model->getMin().x, render->_model->getMin().y, render->_model->getMin().z);

        // Model box collider size
        boxSize.x = render->_model->getSize().x;
        boxSize.y = render->_model->getSize().y;
        boxSize.z = render->_model->getSize().z;

        // Model box collider center
        boxCenter = boxPos;
        boxCenter.x += (boxSize.x / 2.0f);
        boxCenter.y += (boxSize.y / 2.0f);
        boxCenter.z += (boxSize.z / 2.0f);

        // Convert box collider pos and center to world position
        boxPos += transform->pos;
        boxCenter += transform->pos;

        // Calculate vector from near plane to far plane
        glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

        // Calculate the length of this vector
        float rayLength = glm::distance(nearPoint, farPoint);

        // Calculate vector from near plane to object
        glm::vec3 objDir = boxCenter - nearPoint;

        // Project objDir onto rayDir vector
        // It gives us a scalar that is the length of objDir onto rayDir
        float scalar = glm::dot(objDir, rayDir);


        // Find closest point
        glm::vec3 closestPoint;

        // The projected objDir length is less or equal to 0 (the projected vector objDir points backward)
        // (So the point is the nearPoint or before the nearPoint)
        if (scalar <= 0.0f)
            closestPoint = nearPoint;
        // The projected objDir length is greater or equal than the vector that goes from near plane to far plane
        // (So the point is the farPoint or behind the farPoint)
        else if (scalar >= rayLength)
            closestPoint = farPoint;
        // The point is between the near plane and the far plane
        else
            closestPoint = nearPoint + (rayDir * scalar);

        // Check if the point is in the box of the entity selection
        if (Collisions::pointVSAABB(closestPoint, boxPos, boxSize))
        {
            // Calculate point distance
            float closestPointDist = glm::distance(nearPoint, closestPoint);

            // The ray can select multiple entities, we need to store the closest one
            if (!selectedEntity || closestPointDist < selectedEntityDist)
            {
                selectedEntityDist = closestPointDist;
                selectedEntity = entity;
            }
        }
    }

    return (selectedEntity);
}
