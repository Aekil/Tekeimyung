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

#define BLENDING_MODES(PROCESS)                 \
    PROCESS(GL_ZERO)                            \
    PROCESS(GL_ONE)                             \
    PROCESS(GL_SRC_COLOR)                       \
    PROCESS(GL_ONE_MINUS_SRC_COLOR)             \
    PROCESS(GL_DST_COLOR)                       \
    PROCESS(GL_ONE_MINUS_DST_COLOR)             \
    PROCESS(GL_SRC_ALPHA)                       \
    PROCESS(GL_ONE_MINUS_SRC_ALPHA)             \
    PROCESS(GL_DST_ALPHA)                       \
    PROCESS(GL_ONE_MINUS_DST_ALPHA)             \

#define GENERATE_BLEND_STRING(ENUM) #ENUM,
#define GENERATE_BLEND_CONDITION_STRING(ENUM)   \
    if (blendEnum == ENUM)                      \
        return #ENUM;

#define GENERATE_BLEND_CONDITION_ENUM(ENUM)     \
    if (blendString == #ENUM)                    \
        return ENUM;

class Material final: public Resource
{
public:
    struct sMaterialData
    {
        glm::vec4       ambient;
        glm::vec4       diffuse;
        int             texturesTypes;
        int             faceCamera;
    };

public:
    Material(bool isModelMaterial = true);
    Material(const Material& material);
    ~Material() {}

    Material&           operator=(const Material& material);

    static Material*    loadFromAssimp(aiMaterial* assimpMaterial, const std::string& path);
    void                bind();

    bool                isModelMaterial() const;

    void                setTexture(Texture::eType type, Texture* texture);
    Texture*            getTexture(Texture::eType type) const;

    bool                loadFromFile(const std::string& fileName) override final;

    static Resource::eType      getResourceType() { return Resource::eType::MATERIAL; }

    static GLenum       getBlendEnumFromString(const std::string& blendString);
    static const char*  getBlendStringFromEnum(GLenum blendEnum);
    static std::vector<const char*>& getBlendModes();

    const glm::vec4&    getAmbient() const;
    const glm::vec4&    getDiffuse() const;
    int                 isFacingCamera() const;

    void                setAmbient(const glm::vec4& ambient);
    void                setDiffuse(const glm::vec4& diffuse);
    void                isFacingCamera(bool faceCamera);

private:
    void                needUpdate();

public:
    bool                transparent{false};
    GLenum              srcBlend;
    GLenum              dstBlend;

private:
    sMaterialData       _data;
    UniformBuffer       _ubo;

    // Need to update ubo data
    bool                _needUpdate;

    glm::vec4           _ambient;
    glm::vec4           _diffuse;
    int                 _faceCamera;
    int                 _texturesTypes;

private:
    std::unordered_map<Texture::eType, Texture*> _textures;

private:
    bool                _isModelMaterial;
};
