/**
* @Author   Guillaume Labey
*/

#pragma once

#include <assimp/scene.h>
#include <glm/vec4.hpp>
#include <unordered_map>

#include <Engine/Graphics/UniformBuffer.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Utils/Resource.hpp>
#include <Engine/Utils/Helper.hpp>

#define MAT_OPTIONS(PROCESS)                    \
    PROCESS(TEXTURE_AMBIENT, 1)                 \
    PROCESS(TEXTURE_DIFFUSE, 2)                 \
    PROCESS(FACE_CAMERA, 4)                     \
    PROCESS(BLOOM, 8)                           \
    PROCESS(TEXTURE_BLOOM, 16)                  \
    PROCESS(TEXTURE_BLOOM_ALPHA, 32)            \

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
        glm::vec4       bloom;
    };

    REGISTER_ENUM(eOption, uint8_t, MAT_OPTIONS)

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
    const glm::vec4&    getBloom() const;
    bool                isFacingCamera() const;
    bool                hasBloom() const;

    void                setAmbient(const glm::vec4& ambient);
    void                setDiffuse(const glm::vec4& diffuse);
    void                setBloom(const glm::vec4& bloom);
    void                isFacingCamera(bool faceCamera);
    void                hasBloom(bool bloom);

    int                 getOptions();

private:
    void                needUpdate();

public:
    bool                wireframe{false};
    bool                transparent{false};
    GLenum              srcBlend;
    GLenum              dstBlend;

private:
    sMaterialData       _data;
    UniformBuffer       _ubo;

    // Need to update ubo data
    bool                _needUpdate;
    bool                _optionsFlagDirty{true};

    glm::vec4           _ambient;
    glm::vec4           _diffuse;
    glm::vec4           _bloom;
    bool                _faceCamera{false};
    bool                _hasBloom{false};

    int                 _options{0};

private:
    std::unordered_map<Texture::eType, Texture*> _textures;

private:
    bool                _isModelMaterial;
};

REGISTER_ENUM_MANAGER(Material::eOption, MAT_OPTIONS)

inline int operator~(const Material::eOption& rhs) {
    return (~static_cast<int>(rhs));
}

inline int operator|(int& lhs, const Material::eOption& rhs) {
    return (lhs | static_cast<int>(rhs));
}

inline int& operator|=(int& lhs, const Material::eOption& rhs) {
    return (lhs = (lhs | static_cast<int>(rhs)));
}

inline int operator&(int& lhs, const Material::eOption& rhs) {
    return (lhs & static_cast<int>(rhs));
}

inline int& operator&=(int& lhs, const Material::eOption& rhs) {
    return (lhs = (lhs & static_cast<int>(rhs)));
}
