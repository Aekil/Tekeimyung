/**
* @Author   Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Components.hh>

class SidebarText final : public BaseScript
{
public:
    SidebarText() {};
    ~SidebarText() {};

public:
    void start() override final;
    void update(float dt) override final;

public:
    void setParentPos(glm::vec2);

private:
    glm::vec2 _parentPos;
};

REGISTER_SCRIPT(SidebarText);