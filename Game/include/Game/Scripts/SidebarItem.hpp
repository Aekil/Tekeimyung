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
    void start() override final;
    void update(float dt) override final;
};

REGISTER_SCRIPT(SidebarItem);