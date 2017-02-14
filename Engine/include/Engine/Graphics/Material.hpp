/**
* @Author   Guillaume Labey
*/

#pragma once

#include <assimp/scene.h>
#include <glm/vec4.hpp>
#include <unordered_map>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Utils/Resource.hpp>

class Material final: public Resource
{
public:
    typedef struct
    {
        glm::vec4       ambient;
        glm::vec4       diffuse;
        int             texturesTypes;
    }                   Constants;

public:
    Material(bool isModelMaterial = true);
    Material(const Material& material);
    ~Material() {}

    Material&           operator=(const Material& material);

    static Material*    loadFromAssimp(aiMaterial* assimpMaterial, const std::string& path);
    void                bind(const ShaderProgram& shaderProgram);
    bool                isModelMaterial() const;

    void                setTexture(Texture::eType type, Texture* texture);
    Texture*            getTexture(Texture::eType type) const;

    bool                loadFromFile(const std::string& fileName) override final;

    static Resource::eType      getResourceType() { return Resource::eType::MATERIAL; }

public:
    Constants           _constants;
    UniformBuffer       _ubo;

    // Need to update ubo data
    bool                _needUpdate;

private:
    std::unordered_map<Texture::eType, Texture*> _textures;

private:
    bool                _isModelMaterial;
};
