/**
* @Author   Guillaume Labey
*/

#include <Engine/Debug/Logger.hpp>
#include <dirent.h>
#include <fstream>
#include <algorithm>
#include <vector>

#include <Engine/Graphics/UI/Font.hpp>
#include <Engine/Graphics/Geometries/Geometry.hpp>
#include <Engine/Graphics/Material.hpp>
#include <Engine/Graphics/Model.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/File.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Engine/Utils/ResourceManager.hpp>

ResourceManager*   ResourceManager::_ResourceManager = nullptr;

ResourceManager::ResourceManager()
{
    // Add empty name for empty choice
    _resourcesNames[Resource::eType::TEXTURE].push_back("");

    _defaultResources[Resource::eType::MODEL] = "default.DAE";
    _defaultResources[Resource::eType::MATERIAL] = "geometry_default.mat";
}

ResourceManager::~ResourceManager() {}

void    ResourceManager::loadResources(const std::string& directory)
{
    DIR* dir;
    struct dirent* ent;
    ResourceManager* ResourceManager = ResourceManager::getInstance();
    std::vector<std::string> texturesExtensions = { TEXTURES_EXT };
    std::vector<std::string> modelsExtensions = { MODELS_EXT };
    std::vector<std::string> materialsExtensions = { MATERIALS_EXT };
    std::vector<std::string> soundsExtensions = { SOUNDS_EXT };

    dir = opendir(directory.c_str());
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open resource directory \"%s\"", directory.c_str());

    while ((ent = readdir(dir)) != NULL)
    {
        // No file extension, is directory
        if (std::string(ent->d_name).find(".") == std::string::npos)
        {
            // Load directory
            std::string directoryPath = std::string(directory).append("/").append(ent->d_name);
            loadResources(directoryPath);
        }
        else
        {
            std::string file = std::string(directory).append("/").append(ent->d_name);
            std::string extension = ResourceManager::getFileExtension(ent->d_name);
            std::string basename = getBasename(file);

            extension = Helper::lowerCaseString(extension);
            // Texture resource
            if (std::find(texturesExtensions.cbegin(), texturesExtensions.cend(), extension) != texturesExtensions.cend())
            {
                // Textures could be loaded from model (It could conflict)
                getOrLoadResource<Texture>(file);
            }
            // Model resource
            else if (std::find(modelsExtensions.cbegin(), modelsExtensions.cend(), extension) != modelsExtensions.cend())
            {
                loadResource<Model>(file);
            }
            // Materials resource
            else if (std::find(materialsExtensions.cbegin(), materialsExtensions.cend(), extension) != materialsExtensions.cend())
            {
                loadResource<Material>(file);
            }
            else if (std::find(soundsExtensions.cbegin(), soundsExtensions.cend(), extension) != soundsExtensions.cend())
            {
                loadSound(basename, file);
            }
            else if (extension == "ttf")
            {
                loadResource<Font>(file);
            }
        }
    }

    closedir(dir);
}

ResourceManager*   ResourceManager::getInstance()
{
    if (!_ResourceManager)
    {
        _ResourceManager = new ResourceManager();
    }

    return (_ResourceManager);
}

std::string  ResourceManager::getFileExtension(const std::string& fileName)
{
    std::size_t size = fileName.rfind('.', fileName.length());
    if (size != std::string::npos)
        return (fileName.substr(size + 1, fileName.length() - size));

    return ("");
}

std::string ResourceManager::getBasename(const std::string& fileName)
{
    size_t basenameOcur;
    std::string basename = fileName;

    basenameOcur = fileName.find_last_of('/');
    if (basenameOcur != std::string::npos)
    {
        basename = fileName.substr(basenameOcur + 1);
    }

    return (basename);
}

const std::vector<ResourceManager::sSoundStrings>&  ResourceManager::getSoundsStrings() const
{
    return (_soundsStrings);
}

void    ResourceManager::loadSound(const std::string basename, const std::string& fileName)
{
    // Add sound string to Resource Manager
    sSoundStrings soundString;
    soundString.path = fileName;
    soundString.name = basename;
    _soundsStrings.push_back(soundString);

    // Load sound in Sound Manager
    //SoundManager::getInstance()->registerSound(fileName);
}

template<typename T>
T*  ResourceManager::getDefaultResource()
{
    auto defaultResourceName = _defaultResources.find(T::getResourceType());
    if (defaultResourceName == _defaultResources.end())
    {
        return (nullptr);
    }

    auto& resourceMap = _resources[T::getResourceType()];
    auto defaultResource = resourceMap.find(defaultResourceName->second);
    if (defaultResource == resourceMap.end())
    {
        LOG_WARN("Can't find default resource %s", defaultResourceName->second);
        return (nullptr);
    }
    return (static_cast<T*>(defaultResource->second.get()));
}

template<typename T>
T*  ResourceManager::getResource(const std::string& path, bool useDefaultIfNotFound)
{
    // Resources are stored with their basename
    std::string name = getBasename(path);

    // The resource is not loaded
    auto& resourceMap = _resources[T::getResourceType()];
    if (resourceMap.find(name) == resourceMap.end())
    {
        if (useDefaultIfNotFound)
        {
            LOG_WARN("Can't find resource %s, loading default resource", path.c_str());
            return (getDefaultResource<T>());
        }
        else
        {
            return (nullptr);
        }
    }

    auto& resource = resourceMap.at(name);
    return (static_cast<T*>(resource.get()));
}

template<typename T>
T*  ResourceManager::getOrLoadResource(const std::string& path)
{
    // Pass false so we don't get the default resource if it's not found
    // Because we want to load it
    T* resource = getResource<T>(path, false);

    // The resource is not loaded
    if (!resource)
    {
        return loadResource<T>(path);
    }

    return (resource);
}

template<typename T>
T*  ResourceManager::loadResource(const std::string& path)
{
    std::string name = getBasename(path);
    std::unique_ptr<T> resource = std::make_unique<T>();

    if (!resource->loadFromFile(path))
    {
        LOG_WARN("Can't load resource %s, loading default resource", path.c_str());
        return (getDefaultResource<T>());
    }

    auto& resourceMap = _resources[T::getResourceType()];

    resource->setId(name);
    resource->setPath(path);
    resourceMap[name] = std::move(resource);
    // TODO: Allocate char* for resource name
    _resourcesNames[T::getResourceType()].push_back(resourceMap.find(name)->first.c_str());

    return (static_cast<T*>(resourceMap[name].get()));
}

template<typename T>
T*  ResourceManager::registerResource(std::unique_ptr<T> resource, const std::string& path)
{
    std::string name = getBasename(path);
    auto& resourceMap = _resources[T::getResourceType()];

    if (resourceMap[name] != nullptr)
    {
        LOG_WARN("ResourceManager::registerResource: registering resource \"%s\" that does already exists", name.c_str());
    }

    resource->setId(name);
    resource->setPath(path);
    resourceMap[name] = std::move(resource);
    // TODO: Allocate char* for resource name
    _resourcesNames[T::getResourceType()].push_back(resourceMap.find(name)->first.c_str());

    return (static_cast<T*>(resourceMap[name].get()));
}

template<typename T>
T* ResourceManager::getOrCreateResource(const std::string& path)
{
    T* resource = getResource<T>(path);

    // The resource is not loaded
    if (!resource)
    {
        return registerResource<T>(std::make_unique<T>(), path);
    }

    return (resource);
}

template<typename T>
std::vector<const char*>& ResourceManager::getResourcesNames()
{
    return _resourcesNames[T::getResourceType()];
}

template Model*  ResourceManager::getOrLoadResource<Model>(const std::string& path);
template Texture*  ResourceManager::getOrLoadResource<Texture>(const std::string& path);
template Geometry*  ResourceManager::getOrLoadResource<Geometry>(const std::string& path);
template Material*  ResourceManager::getOrLoadResource<Material>(const std::string& path);
template File*  ResourceManager::getOrLoadResource<File>(const std::string& path);
template Font*  ResourceManager::getOrLoadResource<Font>(const std::string& path);

template Model*  ResourceManager::getDefaultResource<Model>();
template Texture*  ResourceManager::getDefaultResource<Texture>();
template Geometry*  ResourceManager::getDefaultResource<Geometry>();
template Material*  ResourceManager::getDefaultResource<Material>();
template File*  ResourceManager::getDefaultResource<File>();
template Font*  ResourceManager::getDefaultResource<Font>();

template Model*  ResourceManager::getResource<Model>(const std::string& path, bool useDefaultIfNotFound);
template Texture*  ResourceManager::getResource<Texture>(const std::string& path, bool useDefaultIfNotFound);
template Geometry*  ResourceManager::getResource<Geometry>(const std::string& path, bool useDefaultIfNotFound);
template Material*  ResourceManager::getResource<Material>(const std::string& path, bool useDefaultIfNotFound);
template File*  ResourceManager::getResource<File>(const std::string& path, bool useDefaultIfNotFound);
template Font*  ResourceManager::getResource<Font>(const std::string& path, bool useDefaultIfNotFound);

template Model*  ResourceManager::registerResource<Model>(std::unique_ptr<Model> resource, const std::string& path);
template Texture*  ResourceManager::registerResource<Texture>(std::unique_ptr<Texture> resource, const std::string& path);
template Geometry*  ResourceManager::registerResource<Geometry>(std::unique_ptr<Geometry> resource, const std::string& path);
template Material*  ResourceManager::registerResource<Material>(std::unique_ptr<Material> resource, const std::string& path);
template File*  ResourceManager::registerResource<File>(std::unique_ptr<File> resource, const std::string& path);

template File*  ResourceManager::getOrCreateResource<File>(const std::string& path);

template std::vector<const char*>&  ResourceManager::getResourcesNames<Model>();
template std::vector<const char*>&  ResourceManager::getResourcesNames<Texture>();
template std::vector<const char*>&  ResourceManager::getResourcesNames<Material>();
template std::vector<const char*>&  ResourceManager::getResourcesNames<Font>();
