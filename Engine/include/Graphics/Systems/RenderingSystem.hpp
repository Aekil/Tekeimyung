#pragma once

#include <unordered_map>
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/Sprite.hpp"
#include "System.hpp"
#include "Core/Components.hh"
#include "Core/Map.hpp"

class RenderingSystem: public System
{
public:
    RenderingSystem(Map* map);
    virtual ~RenderingSystem();

    virtual void update(EntityManager& em, float elapsedTime);
    virtual bool                            init();

private:
    Sprite*                                 getSprite(Entity* entity);

private:
    // Shader program
    ShaderProgram                           _shaderProgram;

    // Render System entities
    std::unordered_map<int, Sprite*>        _renderEntities;

    Map*                                    _map;
};
