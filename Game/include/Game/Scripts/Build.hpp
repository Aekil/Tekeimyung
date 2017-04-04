/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class Build : public BaseScript
{
public:
    Build() = default;
    ~Build() = default;
public:
    virtual void start() override final;
    virtual void update(float dt) override final;
    void setTile(const Entity*);

private:
    void checkBuildableZone();
    void buildInput();

private:
    const Entity* _tile;
    Entity* _preview;

    sTransformComponent* _transform;
    sRenderComponent* _render;
    std::map<std::string, std::vector<std::string>> _buildableItems;
    std::map<std::string, int> _currentIdx;
    bool _buildEnabled;
    float _buildableRadius;
};

REGISTER_SCRIPT(Build);
