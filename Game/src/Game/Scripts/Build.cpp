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
    // BlockBrown
    this->_buildableItems["BlockBrown"].push_back("TRAP_NEEDLE");
    this->_buildableItems["BlockBrown"].push_back("TRAP_CUTTER");
    this->_buildableItems["BlockBrown"].push_back("TRAP_FIRE");
    this->_buildableItems["BlockBrown"].push_back("BLOCK_GREEN");

    // BlockGreen
    this->_buildableItems["BlockGreen"].push_back("TOWER_FIRE");

    this->_currentIdx["BlockGreen"] = 0;
    this->_currentIdx["BlockBrown"] = 0;

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
    }
}

void Build::update(float dt)
{
    this->buildInput();
    this->checkBuildableZone();
}

void Build::buildInput()
{
    if (this->_buildEnabled && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED && this->_tile != nullptr)
    {
        auto& position = this->_tile->getComponent<sTransformComponent>()->getPos();

        auto entity = this->Instantiate(this->_buildableItems[this->_tile->getTag()][this->_currentIdx[this->_tile->getTag()]], glm::vec3(position.x, position.y + 12.5f, position.z));

        if (entity->getTag() != "BlockGreen")
        {
            auto previewRenderer = entity->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }
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

    if (this->keyboard[Keyboard::eKey::B] == Keyboard::eKeyState::KEY_PRESSED)
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

    const auto& tilesBrown = em->getEntitiesByTag("BlockBrown");
    const auto& tilesGreen = em->getEntitiesByTag("BlockGreen");

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
