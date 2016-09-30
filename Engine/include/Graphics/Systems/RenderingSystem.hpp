#pragma once

#include <unordered_map>
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/Texture.hpp"
#include "System.hpp"
#include <glm/vec3.hpp>
#include "Core/Components.hh"

typedef struct
{
    // OpenGL Buffer
    Buffer      buffer;

    // Entity texture
    Texture*    texture;

    // Entity position
    glm::vec3   pos;
}               RenderEntity;

class RenderingSystem: public System
{
public:
    RenderingSystem();
    virtual ~RenderingSystem();

    virtual void update(EntityManager& em, float elapsedTime);
    virtual bool                            init();
    glm::vec3                               posToIso(sPositionComponent *position, sRenderComponent *sprite) const;

private:
    RenderEntity&                           getRenderEntity(Entity* entity);

private:
    // Shader program
    ShaderProgram                           _shaderProgram;

    // Render System entities
    std::unordered_map<int, RenderEntity*>   _renderEntities;
};