/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Engine/Physics/Collisions.hpp>
#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/Build.hpp>
#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/Spawner.hpp>
#include <Game/Scripts/Tile.hpp>
#include <Game/Scripts/Teleport.hpp>

void Build::start()
{
    // TileFloor
    this->_buildableItems["TileFloor"].push_back("TRAP_NEEDLE");
    this->_buildableItems["TileFloor"].push_back("TRAP_CUTTER");
    this->_buildableItems["TileFloor"].push_back("TRAP_FIRE");
    this->_buildableItems["TileFloor"].push_back("TILE_BASE_TURRET");

    // TileBaseTurret
    this->_buildableItems["TileBaseTurret"].push_back("TOWER_FIRE");

    this->_currentIdx["TileBaseTurret"] = 0;
    this->_currentIdx["TileFloor"] = 0;

    this->_buildEnabled = false;
    this->_buildableRadius = 7.7f;

    this->_tile = nullptr;
    this->_preview = nullptr;

    this->_transform = this->getComponent<sTransformComponent>();
    this->_render = this->getComponent<sRenderComponent>();

    // Get GameManager
    {
        auto em = EntityFactory::getBindedEntityManager();
        Entity* gameManager = em->getEntityByTag("GameManager");
        if (!gameManager)
        {
            LOG_WARN("Can't find entity with GameManager tag");
            return;
        }

        auto scriptComponent = gameManager->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on GameManager entity");
            return;
        }

        _gameManager = scriptComponent->getScript<GameManager>("GameManager");
        if (!_gameManager)
        {
            LOG_WARN("Can't find GameManager");
        }
    }
}

void Build::update(float dt)
{
    this->buildInput();
    this->checkBuildableZone();
}

void Build::updateSpawnersPaths(const glm::ivec2& tilePos)
{
    if (!_gameManager)
    {
        LOG_WARN("Can't update spawners paths because there is no GameManager");
        return;
    }

    // This is not a spawner path, no need to update the paths
    if (!_gameManager->spawnersPaths[tilePos.x][tilePos.y])
    {
        return;
    }
    else
    {
        LOG_INFO("UPDATE SPAWNERS PATHS");
    }

    // Clear spawners paths map before update
    std::memset(_gameManager->spawnersPaths,
                0,
                sizeof(_gameManager->spawnersPaths[0][0]) * _gameManager->mapSizeX * _gameManager->mapSizeZ);

    auto em = EntityFactory::getBindedEntityManager();
    const auto& spawners = em->getEntitiesByTag("Spawner");

    // Update paths
    for (auto &spawner : spawners)
    {
        auto scriptComponent = spawner->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on Spawner entity");
            continue;
        }
        if (!scriptComponent->enabled)
            continue;

        auto spawnerScript = scriptComponent->getScript<Spawner>("Spawner");
        if (!spawnerScript)
        {
            LOG_WARN("Can't find Spawner script on scriptComponent");
            continue;
        }

        spawnerScript->updateClosestPath();
    }
}

void Build::buildInput()
{
    if (this->_buildEnabled && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED && this->_tile != nullptr)
    {
        auto& position = this->_tile->getComponent<sTransformComponent>()->getPos();

        auto entity = this->Instantiate(this->_buildableItems[this->_tile->getTag()][this->_currentIdx[this->_tile->getTag()]], glm::vec3(position.x, position.y + 12.5f, position.z));

        if (entity->getTag() != "TileBaseTurret")
        {
            auto previewRenderer = entity->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }

        glm::ivec2 tilePos;
        sTransformComponent* tileTransform = _tile->getComponent<sTransformComponent>();

        tilePos.x = static_cast<int>(std::ceil(tileTransform->getPos().x) / 25.0f);
        tilePos.y = static_cast<int>(tileTransform->getPos().z / 25.0f);
        _gameManager->firstLayerPattern[tilePos.x][tilePos.y] = 0;
        updateSpawnersPaths(tilePos);
    }

    if (this->_buildEnabled && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_2] == Mouse::eButtonState::CLICK_PRESSED && this->_tile != nullptr)
    {
        this->_currentIdx[this->_tile->getTag()]++;

        if (this->_currentIdx[this->_tile->getTag()] >= this->_buildableItems[this->_tile->getTag()].size())
            this->_currentIdx[this->_tile->getTag()] = 0;

        if (this->_tile != nullptr && this->_preview != nullptr)
        {
            this->Destroy(this->_preview);

            auto& position = this->_tile->getComponent<sTransformComponent>()->getPos();
            this->_preview = this->Instantiate(this->_buildableItems[this->_tile->getTag()][this->_currentIdx[this->_tile->getTag()]], glm::vec3(position.x, position.y + 12.5f, position.z));
            auto previewRenderer = this->_preview->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }
    }

    if (this->keyboard[Keyboard::eKey::E] == Keyboard::eKeyState::KEY_PRESSED)
    {
        this->_buildEnabled = !this->_buildEnabled;
        if (!this->_buildEnabled && this->_preview)
        {
            this->Destroy(this->_preview);
            this->_preview = nullptr;
        }
    }
}

void Build::setTile(const Entity* tile)
{
    if (this->_tile != tile)
    {
        if (this->_preview != nullptr)
        {
            this->Destroy(this->_preview);
            this->_preview = nullptr;
        }

        this->_tile = tile;

        if (this->_tile != nullptr && this->_tile->getComponent<sScriptComponent>()->getScript<Tile>("Tile")->isBuildable())
        {
            auto& position = this->_tile->getComponent<sTransformComponent>()->getPos();
            this->_preview = this->Instantiate(this->_buildableItems[this->_tile->getTag()][this->_currentIdx[this->_tile->getTag()]], glm::vec3(position.x, position.y + 12.5f, position.z));
            auto previewRenderer = this->_preview->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }
    }
}

void Build::checkBuildableZone()
{
    auto em = EntityFactory::getBindedEntityManager();

    const auto& tilesBrown = em->getEntitiesByTag("TileFloor");
    const auto& tilesGreen = em->getEntitiesByTag("TileBaseTurret");

    for (auto &tile : tilesBrown)
    {
        auto box = tile->getComponent<sBoxColliderComponent>();
        if (box != nullptr)
        {
            auto pos = tile->getComponent<sTransformComponent>()->getPos();
            auto scriptComponent = tile->getComponent<sScriptComponent>();
            auto transform = tile->getComponent<sTransformComponent>();

            if (!scriptComponent)
                continue;

            Tile* tile = scriptComponent->getScript<Tile>("Tile");
            if (!tile)
                continue;

            tile->setBuildable(false);

            if (!_buildEnabled)
                continue;

            if (Collisions::sphereVSAABB(_transform->getPos(), this->_buildableRadius * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                tile->setBuildable(true);
            }
        }
    }

    for (auto &tile : tilesGreen)
    {
        auto box = tile->getComponent<sBoxColliderComponent>();
        if (box != nullptr)
        {
            auto pos = tile->getComponent<sTransformComponent>()->getPos();
            auto scriptComponent = tile->getComponent<sScriptComponent>();

            if (!scriptComponent)
                continue;

            Tile* tile = scriptComponent->getScript<Tile>("Tile");
            if (!tile)
                continue;

            tile->setBuildable(false);

            if (!_buildEnabled)
                continue;

            if (Collisions::sphereVSAABB(_transform->getPos(), this->_buildableRadius * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                tile->setBuildable(true);
            }
        }
    }
}
