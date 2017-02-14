/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Engine/MaterialDebugWindow.hpp>
#include <Engine/Graphics/Geometries/GeometryFactory.hpp>
#include <Engine/Graphics/Material.hpp>

// Textures and models files extensions loaded in RessourceManager::loadResources
#define TEXTURES_EXT    "png", "jpg", "jpeg"
#define MODELS_EXT      "dae", "obj", "fbx"
#define MATERIALS_EXT   "mat"
#define SOUNDS_EXT      "mp3", "wav"

class RessourceManager
{
friend GeometryFactory;
friend Material;
friend MaterialDebugWindow;

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
    RessourceManager();
    ~RessourceManager();

    void                                            loadResources(const std::string& directory);

    void                                            createFile(const std::string& fileName, const std::string& fileContent);
    std::string                                     getFile(const std::string& fileName);
    void                                            saveFile(const std::string& fileName, const std::string fileContent);

    template<typename T>
    T*                                              getResource(const std::string& name);
    template<typename T>
    T*                                              getOrLoadResource(const std::string& name);
    template<typename T>
    T*                                              loadResource(const std::string& name);
    template<typename T>
    std::vector<const char*>&                       getResourcesNames();

    const std::vector<sSoundStrings>&               getSoundsStrings() const;

    static RessourceManager*                        getInstance();
    static std::string                              getFileExtension(const std::string& fileName);

private:
    template<typename T>
    T*                                              registerResource(const std::string& name, std::unique_ptr<T> resource);

    std::string                                     getBasename(const std::string& fileName);
    std::string                                     loadFile(const std::string basename, const std::string& fileName);
    void                                            loadSound(const std::string basename, const std::string& fileName);

private:
    using tResourcesMap =  std::unordered_map<std::string, std::unique_ptr<Resource>>;

    // Store files content with their basename
    std::unordered_map<std::string, sFile>          _files;

    std::unordered_map<Resource::eType, tResourcesMap> _resources;
    std::unordered_map<Resource::eType, std::vector<const char*>> _resourcesNames;

    // Store sounds basename list
    std::vector<sSoundStrings>                              _soundsStrings;

    //Singleton instance
    static RessourceManager*                        _ressourceManager;
};
