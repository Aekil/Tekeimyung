/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class GameManager;

class Build : public BaseScript
{
public:
    Build() = default;
    ~Build() = default;
public:
    virtual void start() override final;
    virtual void update(float dt) override final;
    void setTile(const Entity*);
    void setLayer(int layer);

private:
    void checkBuildableZone();
    void buildInput();

    bool isSameIsland(float x, float y) const;
    bool isTile(int x, int y) const;

private:
    const Entity* _tile;
    Entity* _preview;

    sTransformComponent* _transform;
    sRenderComponent* _render;
    std::vector<std::vector<std::string>> _buildableItems;
    std::vector<std::string> _layersBlock;
    bool _buildEnabled;
    float _buildableRadius;
    int _currentIdx = 0;
    int _layer = 0;
    bool _buildSecondTP = false;

    Entity* firstTp;
    glm::vec3 _firstTpPos;
    GameManager* _gameManager{nullptr};
};

REGISTER_SCRIPT(Build);
