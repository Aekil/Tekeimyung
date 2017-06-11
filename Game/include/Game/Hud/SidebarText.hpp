/**
* @Author   Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/Core/Components/Components.hh>

enum class ePosition : uint8_t
{
    BELOW = 0,
    TOP_LEFT = 1
};

class SidebarText final : public BaseScript
{
public:
    SidebarText() {};
    ~SidebarText() {};

public:
    void start() override final;
    void update(float dt) override final;

public:
    void setParentPos(glm::vec2, ePosition);

private:
    glm::vec2 _parentPos;
};
