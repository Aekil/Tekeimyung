/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Engine/Physics/Collisions.hpp>
#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/Build.hpp>
#include <Game/Scripts/Tile.hpp>

void Build::start()
{
    this->_buildableItems.push_back("TOWER_FIRE");
    this->_buildableItems.push_back("TRAP_NEEDLE");
    this->_buildableItems.push_back("TRAP_CUTTER");
    this->_buildableItems.push_back("TRAP_FIRE");
    this->_buildEnabled = false;
    this->_buildableRadius = 5.7f;
    this->_currentIdx = 0;
    this->_tile = nullptr;
    this->_preview = nullptr;

    this->_transform = this->getComponent<sTransformComponent>();
    this->_render = this->getComponent<sRenderComponent>();
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

            this->Instantiate(this->_buildableItems[this->_currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
        }
    }

    if (this->_buildEnabled && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_2] == Mouse::eButtonState::CLICK_PRESSED)
    {
        this->_currentIdx++;

        if (this->_currentIdx >= this->_buildableItems.size())
            this->_currentIdx = 0;

        if (this->_tile != nullptr && this->_preview != nullptr)
        {
            this->Destroy(this->_preview);

            auto& position = this->_tile->getComponent<sTransformComponent>()->getPos();
            this->_preview = this->Instantiate(this->_buildableItems[this->_currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
            auto previewRenderer = this->_preview->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }
    }

    if (this->keyboard[Keyboard::eKey::B] == Keyboard::eKeyState::KEY_PRESSED)
        this->_buildEnabled = !this->_buildEnabled;
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
            this->_preview = this->Instantiate(this->_buildableItems[this->_currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
            auto previewRenderer = this->_preview->getComponent<sRenderComponent>();
            previewRenderer->ignoreRaycast = true;
        }
    }
}

void Build::checkBuildableZone()
{
    auto em = EntityFactory::getBindedEntityManager();

    const auto& tiles = em->getEntitiesByTag("BlockBrown");
    for (auto &tile : tiles)
    {
        auto box = tile->getComponent<sBoxColliderComponent>();
        if (box != nullptr)
        {
            auto pos = tile->getComponent<sTransformComponent>()->getPos();
            auto render = tile->getComponent<sRenderComponent>();
            auto scriptComponent = tile->getComponent<sScriptComponent>();

            if (!scriptComponent)
                continue;

            Tile* tile = scriptComponent->getScript<Tile>("Tile");
            if (!tile)
                continue;

            render->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            tile->setBuildable(false);

            if (!_buildEnabled)
                continue;

            if (Collisions::sphereVSAABB(_transform->getPos(), this->_buildableRadius * SIZE_UNIT, box->pos + pos, glm::vec3(box->size.x * SIZE_UNIT, box->size.y * SIZE_UNIT, box->size.z * SIZE_UNIT)))
            {
                tile->setBuildable(true);
                render->color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
            }
        }
    }
}