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
    std::vector<std::string> _buildableItems;
    bool _buildEnabled;
    float _buildableRadius;
    int _currentIdx = 0;
    bool _buildSecondTP = false;

    Entity* firstTp;
    glm::vec3 _firstTpPos;
};

REGISTER_SCRIPT(Build);
