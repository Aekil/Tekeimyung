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
    void                                    renderEntity(Entity* entity);
    void                                    renderEntities(EntityManager& em, std::list<uint32_t>::const_iterator& it, uint16_t layer, uint32_t x, uint32_t y);

private:
    Sprite*                                 getSprite(Entity* entity);

private:
    // Shader program
    ShaderProgram                           _shaderProgram;

    // Render System entities
    std::unordered_map<int, Sprite*>        _renderEntities;

    Map*                                    _map;
};
