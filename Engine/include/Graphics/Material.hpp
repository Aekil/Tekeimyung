#pragma once

# include <assimp/scene.h>
#include <glm/vec3.hpp>

#include "Graphics/ShaderProgram.hpp"

class Material
{
public:
    Material();
    ~Material() {}

    bool                loadFromAssimp(aiMaterial* material);
    void                bind(ShaderProgram& shaderProgram) const;

public:
    glm::vec3           diffuse;
};
