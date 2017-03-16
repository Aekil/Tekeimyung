/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Tile final : public BaseScript
{
public:
    Tile() = default;
    ~Tile() = default;

    bool onHover = false;

    void start() override final;
    void update(float dt) override final;

    void onHoverEnter() override final;
    void onHoverExit() override final;

    void setBuildable(bool);
    bool isBuildable();
private:
    bool _buildable = false;

    sRenderComponent* _render;
};

REGISTER_SCRIPT(Tile);
