/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Logger.hpp>
#include <dirent.h>
#include <fstream>
#include <algorithm>
#include <vector>

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
        }
    }

    closedir(dir);
}

void    ResourceManager::setSaveDirectory(const std::string& directory)
{
    _saveDirectory = directory;
}

std::string     ResourceManager::getDirectoryPath(const std::string& directory) const
{
    std::string saveDirectory = _saveDirectory;
    if (saveDirectory.size() == 0)
    {
        EXCEPT(FileNotFoundException, "Cannot get directory path \"%s\". The save directory has not been set", directory.c_str());
    }
    saveDirectory += "/" + directory + "/";
    return (saveDirectory);
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
    // Add sound string to Ressource Manager
    sSoundStrings soundString;
    soundString.path = fileName;
    soundString.name = basename;
    _soundsStrings.push_back(soundString);

    // Load sound in Sound Manager
    SoundManager::getInstance()->registerSound(fileName);
}

template<typename T>
T*  ResourceManager::getResource(const std::string& path)
{
    // Resources are stored with their basename
    std::string name = getBasename(path);

    // The resource is not loaded
    auto& resourceMap = _resources[T::getResourceType()];
    if (resourceMap.find(name) == resourceMap.end())
    {
        return (nullptr);
    }

    auto& resource = resourceMap.at(name);
    return (static_cast<T*>(resource.get()));
}

template<typename T>
T*  ResourceManager::getOrLoadResource(const std::string& path)
{
    T* resource = getResource<T>(path);

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
    auto& resourceMap = _resources[T::getResourceType()];

    // Resource is already loaded
    if (resourceMap[name] != nullptr)
    {
        LOG_WARN("ResourceManager::loadResource: loading resource \"%s\" that does already exists. Path is %s", name.c_str(), path.c_str());
        return (static_cast<T*>(resourceMap[name].get()));
    }

    if (!resource->loadFromFile(path))
    {
        return (nullptr);
    }

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

    // Resource is already registered
    if (resourceMap[name] != nullptr)
    {
        LOG_WARN("ResourceManager::registerResource: registering resource \"%s\" that does already exists. Path is %s", name.c_str(), path.c_str());
        return (static_cast<T*>(resourceMap[name].get()));
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

template Model*  ResourceManager::getResource<Model>(const std::string& path);
template Texture*  ResourceManager::getResource<Texture>(const std::string& path);
template Geometry*  ResourceManager::getResource<Geometry>(const std::string& path);
template Material*  ResourceManager::getResource<Material>(const std::string& path);
template File*  ResourceManager::getResource<File>(const std::string& path);

template Model*  ResourceManager::registerResource<Model>(std::unique_ptr<Model> resource, const std::string& path);
template Texture*  ResourceManager::registerResource<Texture>(std::unique_ptr<Texture> resource, const std::string& path);
template Geometry*  ResourceManager::registerResource<Geometry>(std::unique_ptr<Geometry> resource, const std::string& path);
template Material*  ResourceManager::registerResource<Material>(std::unique_ptr<Material> resource, const std::string& path);
template File*  ResourceManager::registerResource<File>(std::unique_ptr<File> resource, const std::string& path);

template File*  ResourceManager::getOrCreateResource<File>(const std::string& path);

template std::vector<const char*>&  ResourceManager::getResourcesNames<Model>();
template std::vector<const char*>&  ResourceManager::getResourcesNames<Texture>();
template std::vector<const char*>&  ResourceManager::getResourcesNames<Material>();
