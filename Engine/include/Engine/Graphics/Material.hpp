#pragma once

#include <assimp/scene.h>
#include <glm/vec3.hpp>
#include <unordered_map>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Graphics/Texture.hpp>

class Material
{
public:
    typedef struct
    {
        glm::vec3       ambient;
        float           padding;
        glm::vec3       diffuse;
        float           padding2;
        int             texturesTypes;
    }                   Constants;

public:
    Material();
    ~Material() {}

    bool                loadFromAssimp(aiMaterial* material, const std::string& path);
    void                bind(const ShaderProgram& shaderProgram);

public:
    Constants           _constants;
    UniformBuffer       _ubo;

    // Need to update ubo data
    bool                _needUpdate;

    std::unordered_map<Texture::eType, Texture*> _textures;
};
