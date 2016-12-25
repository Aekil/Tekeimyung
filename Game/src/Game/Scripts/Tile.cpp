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

}

void Tile::OnHoverEnter()
{
    auto renderer = this->getComponent<sRenderComponent>();

    renderer->color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

void Tile::OnHoverExit()
{
    auto renderer = this->getComponent<sRenderComponent>();

    renderer->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}