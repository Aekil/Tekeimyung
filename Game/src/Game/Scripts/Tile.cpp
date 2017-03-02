/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Tile.hpp>

void Tile::start()
{
    buildableItems.push_back("TOWER_FIRE");
    buildableItems.push_back("TRAP_NEEDLE");
    buildableItems.push_back("TRAP_CUTTER");
    buildableItems.push_back("TRAP_FIRE");
    _render = this->getComponent<sRenderComponent>();
}

void Tile::update(float dt)
{
    if (this->m_buildable && this->onHover && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
    {
        auto& position = this->getComponent<sTransformComponent>()->getPos();

        this->Instantiate(this->buildableItems[this->currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
    }

    if (this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_2] == Mouse::eButtonState::CLICK_PRESSED)
    {
        this->currentIdx++;

        if (this->currentIdx >= this->buildableItems.size())
            this->currentIdx = 0;
    }

    if (!this->m_buildable && this->preview != nullptr)
    {
        destroyPreview();
    }
    else if (this->m_buildable &&
        this->preview == nullptr &&
        this->onHover)
    {
        displayPreview();
    }
}

void Tile::onHoverEnter()
{
    if (this->m_buildable)
    {
        displayPreview();
    }

    _render->color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    this->onHover = true;
}

void Tile::onHoverExit()
{
    if (this->preview != nullptr)
    {
        destroyPreview();
    }

    _render->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->onHover = false;
}

void Tile::setBuildable(bool buildable)
{
    this->m_buildable = buildable;
}

void Tile::displayPreview()
{
    auto& position = this->getComponent<sTransformComponent>()->getPos();
    this->preview = this->Instantiate(this->buildableItems[this->currentIdx], glm::vec3(position.x, position.y + 12.5f, position.z));
    auto previewRenderer = this->preview->getComponent<sRenderComponent>();
    previewRenderer->ignoreRaycast = true;
}

void Tile::destroyPreview()
{
    this->Destroy(this->preview);
    this->preview = nullptr;
}
