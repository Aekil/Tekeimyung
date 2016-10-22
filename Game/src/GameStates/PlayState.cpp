#include "Graphics/Systems/RenderingSystem.hpp"
#include "Window/Systems/InputSystem.hpp"
#include "Window/Keyboard.hpp"
#include "Physics/MovementSystem.hpp"
#include "Physics/GravitySystem.hpp"
#include "Physics/CollisionSystem.hpp"
#include "Core/Components.hh"
#include "EntityFactory.hpp"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "GameStates/PlayState.hpp"


PlayState::PlayState(): _windowImgui(true) {}

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

    _map = new Map(em, 10, 10, 4);

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            tile = EntityFactory::createEntity(eArchetype::BLOCK_GREEN);
            sPositionComponent* pos = tile->getComponent<sPositionComponent>();
            pos->value.x = x;
            pos->value.y = y;
            pos->z = 0;
            (*_map)[0][y][x] = tile->id;
        }
    }


    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 10; x++) {
            tile = EntityFactory::createEntity(eArchetype::BLOCK_BROWN);
            sPositionComponent* pos = tile->getComponent<sPositionComponent>();
            pos->value.x = x;
            pos->value.y = y;
            pos->z = 1;
            (*_map)[1][y][x] = tile->id;
        }
    }

    for (int y = 8; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            tile = EntityFactory::createEntity(eArchetype::BLOCK_BROWN);
            sPositionComponent* pos = tile->getComponent<sPositionComponent>();
            pos->value.x = x;
            pos->value.y = y;
            pos->z = 1;
            (*_map)[1][y][x] = tile->id;
        }
    }

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
