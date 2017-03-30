/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Engine/Physics/Collisions.hpp>
#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/Build.hpp>
#include <Game/Scripts/Tile.hpp>
#include <Game/Scripts/Teleport.hpp>
#include <Game/Scripts/GameManager.hpp>

void Build::start()
{
    this->_buildableItems.resize(2);

    // Layer 0
    this->_buildableItems[0].push_back("TRAP_NEEDLE");
    this->_buildableItems[0].push_back("TRAP_CUTTER");
    this->_buildableItems[0].push_back("TRAP_FIRE");

    // Layer 1
    this->_buildableItems[1].push_back("TOWER_FIRE");
    this->_buildableItems[1].push_back("TP_FIRST");
    this->_buildableItems[1].push_back("TP_SECOND");

    this->_layersBlock.push_back("BlockBrown");
    this->_layersBlock.push_back("BlockGreen");

    this->_buildEnabled = false;
    this->_buildableRadius = 7.7f;
    this->_currentIdx = 0;
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
    }
}

void Build::update(float dt)
{
    this->buildInput();
    this->checkBuildableZone();
}

void Build::buildInput()
{
    if (this->_buildEnabled && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
    {
        if (this->_tile != nullptr)
        {
            auto& position = this->_tile->getComponent<sTransformComponent>()->getPos();

            auto entity = this->Instantiate(this->_buildableItems[_layer][this->_currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
            auto previewRenderer = entity->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;

            if (this->_buildableItems[_layer][this->_currentIdx] == "TP_FIRST")
            {
                this->_buildSecondTP = true;
                this->_currentIdx = static_cast<int>(this->_buildableItems[_layer].size() - 1);
                this->_firstTpPos = glm::vec3(position.x, position.y + 12.5f, position.z);
                this->firstTp = entity;
            }
            else if (this->_buildableItems[_layer][this->_currentIdx] == "TP_SECOND")
            {
                this->_buildSecondTP = false;
                this->_currentIdx = 0;

                entity->getComponent<sScriptComponent>()->getScript<Teleport>("Teleport")->setTPPos(this->_firstTpPos);
                entity->getComponent<sScriptComponent>()->getScript<Teleport>("Teleport")->setActivity(false);
                this->firstTp->getComponent<sScriptComponent>()->getScript<Teleport>("Teleport")->setTPPos(glm::vec3(position.x, position.y + 12.5f, position.z));
                this->firstTp->getComponent<sScriptComponent>()->getScript<Teleport>("Teleport")->setActivity(true);
            }
        }
    }

    if (this->_buildEnabled && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_2] == Mouse::eButtonState::CLICK_PRESSED && !this->_buildSecondTP)
    {
        this->_currentIdx++;

        if ((_layer == 1 && this->_currentIdx >= this->_buildableItems[_layer].size() - 1) ||
            _layer == 0 && this->_currentIdx >= this->_buildableItems[_layer].size())
            this->_currentIdx = 0;

        if (this->_tile != nullptr && this->_preview != nullptr)
        {
            this->Destroy(this->_preview);

            auto& position = this->_tile->getComponent<sTransformComponent>()->getPos();
            this->_preview = this->Instantiate(this->_buildableItems[_layer][this->_currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
            auto previewRenderer = this->_preview->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }
    }

    if (this->keyboard[Keyboard::eKey::B] == Keyboard::eKeyState::KEY_PRESSED && !this->_buildSecondTP)
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
            this->_preview = this->Instantiate(this->_buildableItems[_layer][this->_currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
            auto previewRenderer = this->_preview->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }
    }
}

void Build::setLayer(int layer)
{
    _layer = layer;
    _currentIdx = 0;
}

void Build::checkBuildableZone()
{
    auto em = EntityFactory::getBindedEntityManager();

    const auto& tiles = em->getEntitiesByTag(this->_layersBlock[this->_layer]);

    for (auto &tile : tiles)
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

            if (!_buildEnabled ||
                (this->_layer == 1 &&
                    this->_buildableItems[_layer][this->_currentIdx] != "TP_SECOND" &&
                    !isSameIsland(transform->getPos().x, transform->getPos().z)))
                continue;

            if (Collisions::sphereVSAABB(_transform->getPos(), this->_buildableRadius * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                tile->setBuildable(true);
            }
        }
    }

    const auto& tilesOtherLayer = em->getEntitiesByTag(this->_layersBlock[this->_layer == 1 ? 0 : 1]);

    for (auto &tile : tilesOtherLayer)
    {
        auto box = tile->getComponent<sBoxColliderComponent>();
        if (box != nullptr)
        {
            auto scriptComponent = tile->getComponent<sScriptComponent>();

            if (!scriptComponent)
                continue;

            Tile* tile = scriptComponent->getScript<Tile>("Tile");
            if (!tile)
                continue;

            tile->setBuildable(false);
        }
    }
}

bool    Build::isSameIsland(float x, float y) const
{
    glm::ivec2 buildPos{
        buildPos.x = static_cast<int>(std::ceil(x) / 25.0f),
        buildPos.y = static_cast<int>(y / 25.0f)
    };
    glm::ivec2 playerPos{
        playerPos.x = static_cast<int>(std::ceil(_transform->getPos().x) / 25.0f),
        playerPos.y = static_cast<int>(_transform->getPos().z / 25.0f)
    };
    glm::ivec2 direction = buildPos - playerPos;
    direction.x = std::min(direction.x, 1);
    direction.x = std::max(direction.x, -1);
    direction.y = std::min(direction.y, 1);
    direction.y = std::max(direction.y, -1);

    while (playerPos.x != buildPos.x ||
        playerPos.y != buildPos.y)
    {
        while (playerPos.y != buildPos.y)
        {
            if (!isTile(playerPos.x, playerPos.y))
                return (false);
            playerPos.y += direction.y;
        }
        if (!isTile(playerPos.x, playerPos.y))
            return (false);
        playerPos.x += direction.x;
    }
    return (true);
}

bool    Build::isTile(int x, int y) const
{
    if (x < 0 ||
        x >= _gameManager->mapSize ||
        y < 0 ||
        y >= _gameManager->mapSize)
    {
        return (true);
    }
    if (_gameManager->secondLayerPattern[x][y] != 1)
    {
        return (false);
    }

    return (true);
}
