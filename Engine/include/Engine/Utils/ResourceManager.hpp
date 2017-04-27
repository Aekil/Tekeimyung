/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Engine/Utils/Resource.hpp>

// Textures and models files extensions loaded in ResourceManager::loadResources
#define TEXTURES_EXT    "png", "jpg", "jpeg"
#define MODELS_EXT      "dae", "obj", "fbx"
#define MATERIALS_EXT   "mat"
#define SOUNDS_EXT      "mp3", "wav"

class EntityFactory;
class GeometryFactory;
class Material;
class MaterialDebugWindow;
class JsonWriter;

class ResourceManager
{
friend EntityFactory;
friend GeometryFactory;
friend Material;
friend MaterialDebugWindow;
friend JsonWriter;

private:
    struct sFile
    {
        std::string content;
        std::string path;
        std::string basename;
    };

public:
    struct sSoundStrings
    {
        std::string     path;
        std::string     name;
    };

public:
    ResourceManager();
    ~ResourceManager();

    void                                            loadResources(const std::string& directory);

    template<typename T>
    T*                                              getResource(const std::string& path);
    template<typename T>
    T*                                              getOrLoadResource(const std::string& path);
    template<typename T>
    T*                                              loadResource(const std::string& path);
    template<typename T>
    std::vector<const char*>&                       getResourcesNames();

    const std::vector<sSoundStrings>&               getSoundsStrings() const;

    static ResourceManager*                        getInstance();
    static std::string                              getFileExtension(const std::string& fileName);

private:
    template<typename T>
    T*                                              registerResource(std::unique_ptr<T> resource, const std::string& path);
    template<typename T>
    T*                                              getOrCreateResource(const std::string& path);

    std::string                                     getBasename(const std::string& fileName);
    void                                            loadSound(const std::string basename, const std::string& fileName);

    template<typename T>
    T*                                              getDefaultResource();

private:
    using tResourcesMap =  std::unordered_map<std::string, std::unique_ptr<Resource>>;

    std::unordered_map<Resource::eType, tResourcesMap> _resources;
    std::unordered_map<Resource::eType, std::vector<const char*>> _resourcesNames;
    std::unordered_map<Resource::eType, const char*> _defaultResources;

    // Store sounds basename list
    std::vector<sSoundStrings>                              _soundsStrings;

    //Singleton instance
    static ResourceManager*                        _ResourceManager;
};
