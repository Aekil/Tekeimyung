#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Graphics/Material.hpp>

Material::Material(): _needUpdate(true)
{
    _constants.diffuse = {0.0f, 0.0f, 0.0f};
    _constants.ambient = {0.0f, 0.0f, 0.0f};
    _constants.texturesTypes = 0;
}

bool    Material::loadFromAssimp(aiMaterial* material, const std::string& path)
{
    aiString file;
    aiColor3D color;

    // Diffuse color
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        _constants.diffuse = glm::vec3(color.r, color.g, color.b);
    }

    // Ambient color
    if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
        _constants.ambient = glm::vec3(color.r, color.g, color.b);
    }

    // Ambient texture
    if (material->GetTextureCount(aiTextureType_AMBIENT) > 0 &&
        material->GetTexture(aiTextureType_AMBIENT, 0, &file) == AI_SUCCESS)
    {
        _constants.texturesTypes |= Texture::eType::AMBIENT;
        _textures[Texture::eType::AMBIENT] = &RessourceManager::getInstance()->getTexture(path + "/" + file.C_Str());
        _textures[Texture::eType::AMBIENT]->setUnit(GL_TEXTURE0);
    }

    // Diffuse texture
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
        material->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
    {
        _constants.texturesTypes |= Texture::eType::DIFFUSE;
        _textures[Texture::eType::DIFFUSE] = &RessourceManager::getInstance()->getTexture(path + "/" + file.C_Str());
        _textures[Texture::eType::DIFFUSE]->setUnit(GL_TEXTURE1);
    }
    return (true);
}

void    Material::bind(const ShaderProgram& shaderProgram)
{
    if (_needUpdate)
    {
        _ubo.update(&_constants, sizeof(_constants));
        _needUpdate = false;
    }

    // Bind ubo
    _ubo.bind(shaderProgram, "material");

    // Bind textures
    if (_constants.texturesTypes & Texture::eType::AMBIENT)
        _textures[Texture::eType::AMBIENT]->bind();
    if (_constants.texturesTypes & Texture::eType::DIFFUSE)
        _textures[Texture::eType::DIFFUSE]->bind();
}
