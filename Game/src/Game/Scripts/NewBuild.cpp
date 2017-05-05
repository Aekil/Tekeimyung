/*
** @Author : Mathieu Chassara
*/

#include    <Engine/EntityFactory.hpp>
#include    <Engine/Physics/Collisions.hpp>

#include    <Game/Scripts/NewBuild.hpp>
#include    <Game/Scripts/Tile.hpp>
#include    <Game/Scripts/TutoManagerMessage.hpp>
#include    <Game/Scripts/Player.hpp>
#include    <Game/Scripts/Spawner.hpp>

void        NewBuild::start()
{
    this->retrieveManagers();
    this->retrievePlayerScript();
    this->initPrices();
    this->_radius = 7.7f;
}

void        NewBuild::update(float deltaTime)
{
    this->checkUserInputs();
}

void        NewBuild::initPrices()
{
    this->_buildingPrices["TILE_BASE_TURRET"] = 40;
    this->_buildingPrices["TILE_WALL"] = 30;
    this->_buildingPrices["TOWER_FIRE"] = 50;
}

bool        NewBuild::isEnabled() const
{
    return (this->_enabled);
}

void        NewBuild::setEnabled(bool enabled)
{
    this->_enabled = enabled;
}

void        NewBuild::disableAll()
{
    auto        em = EntityFactory::getBindedEntityManager();

    this->_enabled = false;
    this->_currentChoice.erase();
    this->_player->setCanShoot(true);
    this->Destroy(this->_preview);
    this->_preview = nullptr;

    const auto& floorTiles = em->getEntitiesByTag("TileFloor");
    for (Entity* floorTile : floorTiles)
    {
        sScriptComponent*   scriptComponent = floorTile->getComponent<sScriptComponent>();

        if (scriptComponent != nullptr)
        {
            Tile*   tileScript = scriptComponent->getScript<Tile>("Tile");

            if (tileScript != nullptr)
                tileScript->setBuildable(false);
        }
    }

    const auto& turretBasetiles = em->getEntitiesByTag("TileBaseTurret");
    for (Entity* turretBaseTile : turretBasetiles)
    {
        sScriptComponent*   scriptComponent = turretBaseTile->getComponent<sScriptComponent>();

        if (scriptComponent != nullptr)
        {
            Tile*   tileScript = scriptComponent->getScript<Tile>("Tile");

            if (tileScript != nullptr)
                tileScript->setBuildable(false);
        }
    }
}

void        NewBuild::setTileHovered(const Entity* tileHovered)
{
    if (!isEnabled())
    {
        this->_tileHovered = tileHovered;
        return;
    }

    if (tileHovered != nullptr && this->_tileHovered != tileHovered)
    {
        this->Destroy(this->_preview);
        this->_preview = nullptr;

        this->_tileHovered = tileHovered;

        sScriptComponent*   scriptComponent = this->_tileHovered->getComponent<sScriptComponent>();

        if (scriptComponent == nullptr)
        {
            LOG_WARN("Could not find %s on Entity %s", "sScriptComponent", "TileFloor");
            return;
        }

        Tile*   tileScript = scriptComponent->getScript<Tile>("Tile");

        if (tileScript == nullptr)
        {
            LOG_WARN("Could not find script %s on Entity %s", "sScriptComponent", "TileFloor");
            return;
        }

        if (tileScript->isBuildable() == true)
        {
            this->updatePreview();
        }
    }
}

void        NewBuild::retrieveManagers()
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* gameManager = em->getEntityByTag("GameManager");

    if (gameManager == nullptr)
    {
        LOG_WARN("Could not find Entity with tag \"%s\"", "GameManager");
        return;
    }

    auto scriptComponent = gameManager->getComponent<sScriptComponent>();

    if (scriptComponent == nullptr)
    {
        LOG_WARN("Could not find %s on Entity %s", "sScriptComponent", "GameManager");
        return;
    }

    // Retrieve GameManager
    {
        this->_gameManager = scriptComponent->getScript<GameManager>("GameManager");

        if (this->_gameManager == nullptr)
        {
            LOG_WARN("Could not find script %s on Entity %s", "GameManager", "GameManager");
        }
    }

    // Retrieve GoldManager
    {
        this->_goldManager = scriptComponent->getScript<GoldManager>(GOLD_MANAGER_TAG);

        if (this->_goldManager == nullptr)
        {
            LOG_WARN("Could not find script %s on Entity %s", GOLD_MANAGER_TAG, "GameManager");
        }
    }
}

void        NewBuild::retrievePlayerScript()
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* player = em->getEntityByTag("Player");

    if (player == nullptr)
    {
        LOG_WARN("Could not find Entity with tag \"%s\"", "Player");
        return;
    }

    auto scriptComponent = player->getComponent<sScriptComponent>();

    if (scriptComponent == nullptr)
    {
        LOG_WARN("Could not find %s on Entity %s", "sScriptComponent", "Player");
        return;
    }

    this->_player = scriptComponent->getScript<Player>("Player");

    if (this->_player == nullptr)
    {
        LOG_WARN("Could not find script %s on Entity %s", "sScriptComponent", "Player");
    }
}

void        NewBuild::checkUserInputs()
{
    if (this->_enabled == true &&
        this->_tileHovered != nullptr &&
        this->_preview != nullptr &&
        this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_RELEASED)
    {
        this->placePreviewedEntity();
    }
    else if (this->_enabled == true &&
        this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_2] == Mouse::eButtonState::CLICK_RELEASED)
    {
        this->disableAll();
    }

#define BUILD_COND(KEY, ITEM, TUTO_COND)                                                \
    if (this->keyboard.getStateMap()[KEY] == Keyboard::eKeyState::KEY_RELEASED &&       \
        TutoManagerMessage::getInstance()->stateOnGoingOrDone(eTutoState::TUTO_COND))   \
    {                                                                                   \
        this->_currentChoice = ITEM;                                                    \
        LOG_DEBUG("Current choice :\t%s", this->_currentChoice.c_str());                \
        if (this->_tileHovered != nullptr)                                              \
            this->updatePreview();                                                      \
    }

    BUILD_ITEMS(BUILD_COND)
#undef BUILD_COND

    if (!this->_currentChoice.empty())
    {
        this->setEnabled(true);
        this->triggerBuildableZone(this->_currentChoice);
        this->_player->setCanShoot(false);
    }
}

void        NewBuild::triggerBuildableZone(const std::string &archetype)
{
    sTransformComponent*    transform = this->getComponent<sTransformComponent>();

    auto        em = EntityFactory::getBindedEntityManager();

    //  Looking for floor tiles
    const auto& floorTiles = em->getEntitiesByTag("TileFloor");
    for (Entity* floorTile : floorTiles)
    {
        auto    box = floorTile->getComponent<sBoxColliderComponent>();

        if (box != nullptr)
        {
            auto    tileTransform = floorTile->getComponent<sTransformComponent>();
            auto    tileScriptComponent = floorTile->getComponent<sScriptComponent>();

            if (tileScriptComponent == nullptr || tileScriptComponent->enabled == false)
                continue;

            Tile*   tileScript = tileScriptComponent->getScript<Tile>("Tile");

            if (tileScript == nullptr)
                continue;

            tileScript->setBuildable(false);

            if (this->isEnabled() == false)
                continue;

            if (Collisions::sphereVSAABB(transform->getPos(),
                this->_radius * SIZE_UNIT, box->pos + tileTransform->getPos(),
                glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                tileScript->setBuildable(true);
            }
            else
            {
                if (tileScript->getEntity() == this->_tileHovered)
                {
                    this->Destroy(this->_preview);
                    this->_preview = nullptr;
                    this->_tileHovered = nullptr;
                }
            }
        }
    }

    //  Looking for turret base tiles
    const auto& turretBasetiles = em->getEntitiesByTag("TileBaseTurret");
    for (Entity* turretBaseTile : turretBasetiles)
    {
        auto    box = turretBaseTile->getComponent<sBoxColliderComponent>();

        if (box != nullptr)
        {
            auto    tileTransform = turretBaseTile->getComponent<sTransformComponent>();
            auto    tileScriptComponent = turretBaseTile->getComponent<sScriptComponent>();

            if (tileScriptComponent == nullptr || tileScriptComponent->enabled == false)
                continue;

            Tile*   tileScript = tileScriptComponent->getScript<Tile>("Tile");

            if (tileScript == nullptr)
                continue;

            tileScript->setBuildable(false);

            if (this->isEnabled() == false)
                continue;

            if (Collisions::sphereVSAABB(transform->getPos(),
                this->_radius * SIZE_UNIT, box->pos + tileTransform->getPos(),
                glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                tileScript->setBuildable(true);
            }
            else
            {
                if (tileScript->getEntity() == this->_tileHovered)
                {
                    this->Destroy(this->_preview);
                    this->_preview = nullptr;
                    this->_tileHovered = nullptr;
                }
            }
        }
    }
}

void        NewBuild::placePreviewedEntity()
{
    if (std::find(this->_alreadyBuiltTile.begin(), this->_alreadyBuiltTile.end(), this->_tileHovered->handle) != this->_alreadyBuiltTile.end())
        return;

    if (!this->_goldManager->removeGolds(this->_buildingPrices[this->_currentChoice]))
        return;

    if (entity->getTag() != "TileBaseTurret")
    {
        auto previewRenderer = entity->getComponent<sRenderComponent>();
        previewRenderer->ignoreRaycast = true;
    }

    auto&   position = this->_tileHovered->getComponent<sTransformComponent>()->getPos();
    auto    entity = this->Instantiate(this->_currentChoice, glm::vec3(position.x, position.y + 12.5f, position.z));
    this->_alreadyBuiltTile.push_back(this->_tileHovered->handle);

    glm::ivec2              tilePos;
    sTransformComponent*    tileTransform = this->_tileHovered->getComponent<sTransformComponent>();

    tilePos.x = static_cast<int>(std::ceil(tileTransform->getPos().x) / 25.0f);
    tilePos.y = static_cast<int>(tileTransform->getPos().z / 25.0f);
    this->_gameManager->map[tilePos.x][tilePos.y] = 0;
    this->updateSpawnersPaths(tilePos);

#define SEND_TUTO_BUILD(KEY, ITEM, TUTO_COND)                                           \
    if (this->_currentChoice == ITEM)                                                   \
    {                                                                                   \
        TutoManagerMessage::getInstance()->sendMessage(eTutoState::TUTO_COND);          \
    }

    BUILD_ITEMS(SEND_TUTO_BUILD)
#undef SEND_TUTO_BUILD
}

void        NewBuild::updateSpawnersPaths(const glm::ivec2& tilePos)
{
    if (this->_gameManager == nullptr)
    {
        LOG_WARN("Can't update spawners paths because there is no GameManager");
        return;
    }

    auto& spawnersPaths = _gameManager->map.getSpawnersPaths();
    // This is not a spawner path, no need to update the paths
    if (!spawnersPaths[tilePos.x][tilePos.y])
    {
        return;
    }
    else
    {
        LOG_INFO("UPDATE SPAWNERS PATHS");
    }

    auto& mapEntities = _gameManager->map.getEntities();
    for (int x = 0; x < _gameManager->map.getWidth(); x++)
    {
        for (int z = 0; z < _gameManager->map.getHeight(); z++)
        {
            Entity* tile = mapEntities[x][z];
            if (tile)
            {
                sRenderComponent* tileRender = tile->getComponent<sRenderComponent>();
                tileRender->color = glm::vec4(1.0f);
            }
        }
    }

    // Clear spawners paths map before update
    spawnersPaths.clear();

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

void            NewBuild::updatePreview()
{
    auto&       position = this->_tileHovered->getComponent<sTransformComponent>()->getPos();
    bool isTower = this->_currentChoice == "TOWER_FIRE";

    this->Destroy(this->_preview);

    // Can only build towers on TileBaseTurret
    if (isTower &&
        this->_tileHovered->getTag() != "TileBaseTurret")
    {
        return;
    }
    // Only towers can be built on TileBaseTurret
    else if (!isTower &&
        this->_tileHovered->getTag() != "TileFloor")
    {
        return;
    }

    this->_preview = this->Instantiate(this->_currentChoice, glm::vec3(position.x, position.y + 12.5f, position.z));

    if (this->_preview != nullptr)
    {
        auto    previewRenderer = this->_preview->getComponent<sRenderComponent>();
        auto    previewScripts = this->_preview->getComponent<sScriptComponent>();

        previewRenderer->ignoreRaycast = true;
        previewScripts->enabled = false;
    }
}
