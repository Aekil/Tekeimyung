/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec2.hpp>

#include <Engine/Core/ScriptFactory.hpp>

class GameManager;

class               Build : public BaseScript
{
public:
    Build() = default;
    ~Build() = default;
public:
    virtual void    start() override final;
    virtual void    update(float dt) override final;
    void            setTile(const Entity*);

private:
    void            checkBuildableZone();
    void            buildInput();
    void            updateSpawnersPaths(const glm::ivec2& tilePos);

private:
    const Entity*   _tile;
    Entity*         _preview;

    sTransformComponent*    _transform;
    sRenderComponent*       _render;

    //  The pair is : <layer, archetype>
    std::map<std::string, std::vector<std::string>> _buildableItems;

    std::map<std::string, int>                      _currentIdx;
    bool    _buildEnabled;
    float   _buildableRadius;

    GameManager* _gameManager = nullptr;
};

REGISTER_SCRIPT(Build);
