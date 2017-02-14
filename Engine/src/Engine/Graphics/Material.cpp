/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Graphics/Material.hpp>

Material::Material(bool isModelMaterial): _needUpdate(true), _isModelMaterial(isModelMaterial)
{
    _constants.diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
    _constants.ambient = {0.3f, 0.3f, 0.3f, 1.0f};
    _constants.texturesTypes = 0;
    _textures[Texture::eType::AMBIENT] = nullptr;
    _textures[Texture::eType::DIFFUSE] = nullptr;
}

Material::Material(const Material& material)
{
    _constants.diffuse = material._constants.diffuse;
    _constants.ambient = material._constants.ambient;
    _constants.texturesTypes = material._constants.texturesTypes;
    _textures = material._textures;
    _isModelMaterial = material._isModelMaterial;
}

Material&   Material::operator=(const Material& material)
{
    _constants.diffuse = material._constants.diffuse;
    _constants.ambient = material._constants.ambient;
    _constants.texturesTypes = material._constants.texturesTypes;
    _textures = material._textures;
    _isModelMaterial = material._isModelMaterial;

    return (*this);
}

Material*    Material::loadFromAssimp(aiMaterial* assimpMaterial, const std::string& path)
{
    aiString file;
    aiColor3D color;
    aiString name;

    if (assimpMaterial->Get(AI_MATKEY_NAME, name) == AI_SUCCESS)
    {
        LOG_INFO("Load material \"%s\"", name.C_Str());
    }
    else
    {
        return (nullptr);
    }

    // Return material if already exists
    {
        Material* material = RessourceManager::getInstance()->getResource<Material>(name.C_Str());
        if (material)
        {
            return (material);
        }
    }

    std::unique_ptr<Material> material = std::make_unique<Material>();

    // Diffuse color
    if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        material->_constants.diffuse = glm::vec4(color.r, color.g, color.b, 1.0f);
    }

    // Ambient color
    if (assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
        material->_constants.ambient = glm::vec4(color.r, color.g, color.b, 1.0f);
    }

    // Ambient texture
    if (assimpMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0 &&
        assimpMaterial->GetTexture(aiTextureType_AMBIENT, 0, &file) == AI_SUCCESS)
    {
        Texture* ambientTexture = RessourceManager::getInstance()->getOrLoadResource<Texture>(path + "/" + file.C_Str());
        material->setTexture(Texture::eType::AMBIENT, ambientTexture);
    }

    // Diffuse texture
    if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
        assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
    {
        Texture* diffuseTexture = RessourceManager::getInstance()->getOrLoadResource<Texture>(path + "/" + file.C_Str());
        material->setTexture(Texture::eType::DIFFUSE, diffuseTexture);
    }

    return (RessourceManager::getInstance()->registerResource<Material>(name.C_Str(), std::move(material)));
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
        _textures[Texture::eType::AMBIENT]->bind(GL_TEXTURE0);
    if (_constants.texturesTypes & Texture::eType::DIFFUSE)
        _textures[Texture::eType::DIFFUSE]->bind(GL_TEXTURE1);
}

bool    Material::isModelMaterial() const
{
    return (_isModelMaterial);
}

void    Material::setTexture(Texture::eType type, Texture* texture)
{
    _constants.texturesTypes |= type;
    _textures[type] = texture;
}

Texture*    Material::getTexture(Texture::eType type) const
{
    return (_textures.at(type));
}

bool        Material::loadFromFile(const std::string& fileName)
{
    JsonReader jsonReader;
    JsonValue parsed;

    if (!jsonReader.parse(fileName, parsed))
        EXCEPT(IOException, "Cannot load material \"%s\"", fileName.c_str());

    _constants.ambient = parsed.getColor4f("ambient", {0.3f, 0.3f, 0.3f, 1.0f});
    _constants.diffuse = parsed.getColor4f("diffuse", {1.0f, 1.0f, 1.0f, 1.0f});

    JsonValue textures(parsed.get("textures", {}));

    // Ambient texture
    {
        std::string texturePath = textures.getString("ambient", "");
        if (texturePath.size() != 0)
        {
            Texture* texture = RessourceManager::getInstance()->getOrLoadResource<Texture>(texturePath);
            setTexture(Texture::eType::AMBIENT, texture);
        }
    }

    // Diffuse texture
    {
        std::string texturePath = textures.getString("diffuse", "");
        if (texturePath.size() != 0)
        {
            Texture* texture = RessourceManager::getInstance()->getOrLoadResource<Texture>(texturePath);
            setTexture(Texture::eType::DIFFUSE, texture);
        }
    }

    _isModelMaterial = false;

    return (true);
}
