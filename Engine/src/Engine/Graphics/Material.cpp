/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Graphics/Material.hpp>

Material::Material(bool isModelMaterial): _isModelMaterial(isModelMaterial)
{
    _diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
    _ambient = {1.0f, 1.0f, 1.0f, 1.0f};
    _bloom = {1.0f, 1.0f, 1.0f, 1.0f};
    srcBlend = GL_SRC_ALPHA;
    dstBlend = GL_ONE_MINUS_SRC_ALPHA;
    _textures[Texture::eType::AMBIENT] = nullptr;
    _textures[Texture::eType::DIFFUSE] = nullptr;
    _textures[Texture::eType::BLOOM] = nullptr;
    _textures[Texture::eType::BLOOM_ALPHA] = nullptr;
    _ubo.init(sizeof(sMaterialData));
    needUpdate();
}

Material::Material(const Material& material): Resource(material)
{
    _diffuse = material._diffuse;
    _ambient = material._ambient;
    _bloom = material._bloom;
    _faceCamera = material._faceCamera;
    _hasBloom = material._hasBloom;
    wireframe = material.wireframe;
    transparent = material.transparent;
    srcBlend = material.srcBlend;
    dstBlend = material.dstBlend;
    _textures = material._textures;
    _isModelMaterial = material._isModelMaterial;
    _ubo.init(sizeof(sMaterialData));
    needUpdate();
}

Material&   Material::operator=(const Material& material)
{
    Resource::operator=(material);
    _diffuse = material._diffuse;
    _ambient = material._ambient;
    _bloom = material._bloom;
    _faceCamera = material._faceCamera;
    _hasBloom = material._hasBloom;
    wireframe = material.wireframe;
    transparent = material.transparent;
    srcBlend = material.srcBlend;
    dstBlend = material.dstBlend;
    _textures = material._textures;
    _isModelMaterial = material._isModelMaterial;
    _ubo.init(sizeof(sMaterialData));
    needUpdate();

    return (*this);
}

bool    Material::operator==(const Material& material)
{
    return (Resource::operator==(material) &&
            _faceCamera == material._faceCamera &&
            _hasBloom == material._hasBloom &&
            transparent == material.transparent &&
            wireframe == material.wireframe &&
            _isModelMaterial == material._isModelMaterial &&
            srcBlend == material.srcBlend &&
            dstBlend == material.dstBlend &&
            _diffuse == material._diffuse &&
            _ambient == material._ambient &&
            _bloom == material._bloom &&
            _textures == material._textures);
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
        Material* material = ResourceManager::getInstance()->getResource<Material>(name.C_Str(), false);
        if (material)
        {
            return (material);
        }
    }

    std::unique_ptr<Material> material = std::make_unique<Material>();

    // Diffuse color
    if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
        material->_diffuse = glm::vec4(color.r, color.g, color.b, 1.0f);
    }

    // Ambient color
    if (assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) {
        material->_ambient = glm::vec4(color.r, color.g, color.b, 1.0f);
    }

    // Ambient texture
    if (assimpMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0 &&
        assimpMaterial->GetTexture(aiTextureType_AMBIENT, 0, &file) == AI_SUCCESS)
    {
        Texture* ambientTexture = ResourceManager::getInstance()->getOrLoadResource<Texture>(path + "/" + file.C_Str());
        material->setTexture(Texture::eType::AMBIENT, ambientTexture);
    }

    // Diffuse texture
    if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
        assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
    {
        Texture* diffuseTexture = ResourceManager::getInstance()->getOrLoadResource<Texture>(path + "/" + file.C_Str());
        material->setTexture(Texture::eType::DIFFUSE, diffuseTexture);
    }

    return (ResourceManager::getInstance()->registerResource<Material>(std::move(material), name.C_Str()));
}

void    Material::bind()
{
    if (_needUpdate)
    {
        _data.ambient = _ambient;
        _data.diffuse = _diffuse;
        _data.bloom = _bloom;

        _ubo.update(&_data, sizeof(sMaterialData));
        _needUpdate = false;
    }

    // Bind ubo
    _ubo.bind();

    // Get updated options
    int options = getOptions();

    // Bind textures
    if (options & eOption::TEXTURE_AMBIENT)
        _textures[Texture::eType::AMBIENT]->bind(GL_TEXTURE0);
    if (options & eOption::TEXTURE_DIFFUSE)
        _textures[Texture::eType::DIFFUSE]->bind(GL_TEXTURE1);
    if (options & eOption::TEXTURE_BLOOM)
        _textures[Texture::eType::BLOOM]->bind(GL_TEXTURE2);
    if (options & eOption::TEXTURE_BLOOM_ALPHA)
        _textures[Texture::eType::BLOOM_ALPHA]->bind(GL_TEXTURE3);
}

bool    Material::isModelMaterial() const
{
    return (_isModelMaterial);
}

void    Material::setTexture(Texture::eType type, Texture* texture)
{
    _textures[type] = texture;
    needUpdate();
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

    _ambient = parsed.getColor4f("ambient", {0.3f, 0.3f, 0.3f, 1.0f});
    _diffuse = parsed.getColor4f("diffuse", {1.0f, 1.0f, 1.0f, 1.0f});
    _bloom = parsed.getColor4f("bloom_color", {1.0f, 1.0f, 1.0f, 1.0f});
    _faceCamera = parsed.getBool("face_camera", false);
    _hasBloom = parsed.getBool("has_bloom", false);
    transparent = parsed.getBool("transparent", false);
    wireframe = parsed.getBool("wireframe", false);
    srcBlend = Material::getBlendEnumFromString(parsed.getString("src_blend", "GL_SRC_ALPHA"));
    dstBlend = Material::getBlendEnumFromString(parsed.getString("dst_blend", "GL_ONE_MINUS_SRC_ALPHA"));

    JsonValue textures(parsed.get("textures", {}));

    try
    {
        // Ambient texture
        {
            std::string texturePath = textures.getString("ambient", "");
            if (texturePath.size() != 0)
            {
                Texture* texture = ResourceManager::getInstance()->getOrLoadResource<Texture>(texturePath);
                setTexture(Texture::eType::AMBIENT, texture);
            }
        }

        // Diffuse texture
        {
            std::string texturePath = textures.getString("diffuse", "");
            if (texturePath.size() != 0)
            {
                Texture* texture = ResourceManager::getInstance()->getOrLoadResource<Texture>(texturePath);
                setTexture(Texture::eType::DIFFUSE, texture);
            }
        }

        // Bloom texture
        {
            std::string texturePath = textures.getString("bloom", "");
            if (texturePath.size() != 0)
            {
                Texture* texture = ResourceManager::getInstance()->getOrLoadResource<Texture>(texturePath);
                setTexture(Texture::eType::BLOOM, texture);
            }
        }

        // Bloom texture alpha
        {
            std::string texturePath = textures.getString("bloom_alpha", "");
            if (texturePath.size() != 0)
            {
                Texture* texture = ResourceManager::getInstance()->getOrLoadResource<Texture>(texturePath);
                setTexture(Texture::eType::BLOOM_ALPHA, texture);
            }
        }
    }
    catch (FileNotFoundException& e) { (void)e; }

    _isModelMaterial = false;
    needUpdate();

    return (true);
}

GLenum  Material::getBlendEnumFromString(const std::string& blendString)
{
    BLENDING_MODES(GENERATE_BLEND_CONDITION_ENUM);

    return (GL_ZERO);
}

const char* Material::getBlendStringFromEnum(GLenum blendEnum)
{
    BLENDING_MODES(GENERATE_BLEND_CONDITION_STRING);

    return ("");
}

std::vector<const char*>&   Material::getBlendModes()
{
    static std::vector<const char*> blendModes = { BLENDING_MODES(GENERATE_BLEND_STRING) };

    return (blendModes);
}

const glm::vec4&    Material::getAmbient() const
{
    return (_ambient);
}

const glm::vec4&    Material::getDiffuse() const
{
    return (_diffuse);
}

const glm::vec4&    Material::getBloom() const
{
    return (_bloom);
}

bool     Material::isFacingCamera() const
{
    return (_faceCamera);
}

bool     Material::hasBloom() const
{
    return (_hasBloom);
}

void    Material::setAmbient(const glm::vec4& ambient)
{
    _ambient = ambient;
    needUpdate();
}

void    Material::setDiffuse(const glm::vec4& diffuse)
{
    _diffuse = diffuse;
    needUpdate();
}

void    Material::setBloom(const glm::vec4& bloom)
{
    _bloom = bloom;
    needUpdate();
}

void    Material::isFacingCamera(bool faceCamera)
{
    _faceCamera = faceCamera;
    needUpdate();
}

void    Material::hasBloom(bool bloom)
{
    _hasBloom = bloom;
    needUpdate();
}

int     Material::getOptions()
{
    if (_optionsFlagDirty)
    {
        _options = 0;
        if (_faceCamera)
            _options |= eOption::FACE_CAMERA;
        if (_textures[Texture::eType::AMBIENT] != nullptr)
            _options |= eOption::TEXTURE_AMBIENT;
        if (_textures[Texture::eType::DIFFUSE] != nullptr)
            _options |= eOption::TEXTURE_DIFFUSE;
        if (_textures[Texture::eType::BLOOM] != nullptr)
            _options |= eOption::TEXTURE_BLOOM;
        if (_textures[Texture::eType::BLOOM_ALPHA] != nullptr)
            _options |= eOption::TEXTURE_BLOOM_ALPHA;
        if (_hasBloom)
            _options |= eOption::BLOOM;

        _optionsFlagDirty = false;
    }

    return (_options);
}

void    Material::needUpdate()
{
    _needUpdate = true;
    _optionsFlagDirty = true;
}
