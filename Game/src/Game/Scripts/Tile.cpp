/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Tile.hpp>

void Tile::Start()
{

}

void Tile::Update(float dt)
{
    if (this->onHover && this->mouse.getStateMap()[Mouse::eButton::MOUSE_BUTTON_1] == Mouse::eButtonState::CLICK_PRESSED)
    {
        auto position = this->getComponent<sTransformComponent>()->pos;

        this->Instantiate("TOWER_FIRE", glm::vec3(position.x, 25.0f, position.z));
    }
}

void Tile::OnHoverEnter()
{
    auto position = this->getComponent<sTransformComponent>()->pos;
    this->preview = this->Instantiate("TOWER_FIRE", glm::vec3(position.x, 25.0f, position.z));

    auto renderer = this->getComponent<sRenderComponent>();

    renderer->color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    this->onHover = true;
}

void Tile::OnHoverExit()
{
    if (this->preview != nullptr)
    {
        this->Destroy(this->preview);
        this->preview = nullptr;
    }

    auto renderer = this->getComponent<sRenderComponent>();

    renderer->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->onHover = false;
}