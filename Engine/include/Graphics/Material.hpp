#pragma once

#include <assimp/scene.h>
#include <glm/vec3.hpp>

#include "Graphics/ShaderProgram.hpp"
#include "Graphics/UniformBuffer.hpp"

class Material
{
public:
    typedef struct
    {
        glm::vec3       ambient;
        float           padding;
        glm::vec3       diffuse;
    }                   Constants;

public:
    Material();
    ~Material() {}

    bool                loadFromAssimp(aiMaterial* material);
    void                bind(const ShaderProgram& shaderProgram);

public:
    glm::vec3           diffuse;
    Constants           _constants;
    UniformBuffer       _ubo;

    // Need to update ubo data
    bool                _needUpdate;
};
