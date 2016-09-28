#pragma once

#include <unordered_map>
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/Texture.hpp"
#include "System.hpp"

typedef struct
{
    // OpenGL Buffer
    Buffer      buffer;
    Texture*    texture;
}               RenderEntity;

class RenderingSystem: public System
{
public:
    RenderingSystem();
    virtual ~RenderingSystem();

    virtual void update(EntityManager& em, float elapsedTime);
    virtual bool                            init();

private:
    RenderEntity&                           getRenderEntity(Entity* entity);

private:
    // Shader program
    ShaderProgram                           _shaderProgram;

    // Render System entities
    std::unordered_map<int, RenderEntity*>   _renderEntities;
};