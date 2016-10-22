#include "Graphics/Systems/RenderingSystem.hpp"
#include "Window/Systems/InputSystem.hpp"
#include "Window/Keyboard.hpp"
#include "Physics/MovementSystem.hpp"
#include "Physics/GravitySystem.hpp"
#include "Physics/CollisionSystem.hpp"
#include "Core/Components.hh"

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
    Entity* player;
    Entity* player2;
    Entity* tower;

    EntityFactory::bindEntityManager(&em);
    player = EntityFactory::createEntity(eArchetype::PLAYER);
    player2 = EntityFactory::createEntity(eArchetype::PLAYER);
    sPositionComponent* posPlayer = player->getComponent<sPositionComponent>();
    sPositionComponent* posPlayer2 = player2->getComponent<sPositionComponent>();
    sInputComponent* input = player2->getComponent<sInputComponent>();

    posPlayer->value.x = 9;
    posPlayer->value.y = 9;
    posPlayer->z = 1;

    posPlayer2->value.x = 6;
    posPlayer2->value.y = 8;
    posPlayer2->z = 1;

    input->moveLeft = Keyboard::eKey::J;
    input->moveRight = Keyboard::eKey::L;
    input->moveUp = Keyboard::eKey::I;
    input->moveDown = Keyboard::eKey::K;

    _map = new Map(em, 20, 15, 4);

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


    // init towers
    createTile(glm::vec3(7, 4, 1), eArchetype::TOWER_FIRE);
    createTile(glm::vec3(7, 7, 1), eArchetype::TOWER_FIRE);

    //(*_map)[1].addEntity(player->id);
    //(*_map)[1].addEntity(player2->id);

    _world.addSystem<InputSystem>();
    //_world.addSystem<GravitySystem>();
    _world.addSystem<MovementSystem>(_map);
    _world.addSystem<CollisionSystem>(_map);
    _world.addSystem<RenderingSystem>(_map);

    return (true);
}

void    PlayState::createEntity(const glm::vec3& pos)
{
    Entity* entity = EntityFactory::createEntity(eArchetype::PLAYER);

    sPositionComponent* posEntity = entity->getComponent<sPositionComponent>();
    posEntity->value.x = pos.x;
    posEntity->value.y = pos.y;
    posEntity->z = pos.z;

    (*_map)[pos.z].addEntity(entity->id);
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

        createEntity(pos);
    }

    GameState::update(elapsedTime);

    return (true);
}
