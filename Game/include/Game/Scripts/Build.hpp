/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Build : public BaseScript
{
public:
    Build() {} = default;
    ~Build() {} = default;
public:
    virtual void start() override final;
    virtual void update(float dt) override final;

private:
    void checkBuildableZone();

private:
    sTransformComponent* _transform;
    sRenderComponent* _render;
    std::vector<std::string> _buildableItems;
    bool _buildEnabled;
    float _buildableRadius;
};

REGISTER_SCRIPT(Build);
