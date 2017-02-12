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

private:
    void displayPreview();
    void destroyPreview();

private:
    Entity* preview = nullptr;
    std::vector<std::string> buildableItems;
    int currentIdx = 0;
    bool m_buildable = false;

    sRenderComponent* _render;
};

REGISTER_SCRIPT(Tile);
