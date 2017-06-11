/*
** @Author : Simon AMBROISE
*/

#pragma once

#include    <Engine/Core/Components/RenderComponent.hh>
#include    <Engine/Core/ScriptFactory.hpp>
#include    <Engine/Graphics/Material.hpp>

class       Tile final : public BaseScript
{
public:
    Tile() = default;
    ~Tile() = default;

    void    start() override final;
    void    update(float dt) override final;

    void    onHoverEnter() override final;
    void    onHoverExit() override final;

    bool    isBuildable();
    void    setBuildable(bool);

private:
    bool    _buildable = false;

    sRenderComponent*   _render;
    Material           _renderMaterial;
    Material*           _buildMaterial;
};
