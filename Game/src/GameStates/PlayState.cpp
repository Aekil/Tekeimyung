#include "Systems/RenderingSystem.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/GravitySystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/AISystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Window/Keyboard.hpp"
#include "Core/Components.hh"
#include "Sound/SoundManager.hpp"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

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

bool    PlayState::init()
{
    EntityManager &em = _world.getEntityManager();

    EntityFactory::bindEntityManager(&em);

    _map = new Map(em, 20, 15, 4);

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
    _world.addSystem<RenderingSystem>(_map);

    // Play sound
    SoundManager::getInstance()->playBkgdMusic("test.mp3", false);

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
    return entity;
}

bool    PlayState::update(float elapsedTime)
{
    bool createEntityButton = false;
    ImGui_ImplGlfwGL3_NewFrame();
    {
        ImGui::SetNextWindowSize(ImVec2(400, 50), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Debug", &_windowImgui);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        createEntityButton = ImGui::Button("Create entity");
        ImGui::End();
    }

    if (createEntityButton)
    {
        CollisionMap* collisionMap = _map->getCollisionMap();
        eColType colType = eColType::CAN_NOT_WALK;

        // Generate random position
        glm::vec3 pos;
        while (colType == eColType::CAN_NOT_WALK)
        {
            pos.x  = static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (_map->getWidth() - 1)));
            pos.y  = static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (_map->getHeight() - 1)));
            pos.z  = std::floor(1 + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (_map->getLayersNb() - 1))));

            colType = (*collisionMap)[pos.z - 1][std::floor(pos.y)][std::floor(pos.x)];
        }

        createEntity(pos, eArchetype::PLAYER);
    }

    GameState::update(elapsedTime);

    return (true);
}
