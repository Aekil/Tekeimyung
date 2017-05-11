/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Components.hh>

class SidebarItem final : public BaseScript
{
public:
    SidebarItem() {};
    ~SidebarItem() {};

public:
    void    start() override final;
    void    update(float dt) override final;

    void    onHoverEnter() override final;
    void    onHoverExit() override final;

    void    setDescription(const std::string& description);

private:
    void    displayTextInfo(bool display);

private:
    sRenderComponent*   _infoRender = nullptr;
    sTextComponent*     _infoText = nullptr;
    std::string         _description;
};

REGISTER_SCRIPT(SidebarItem);
