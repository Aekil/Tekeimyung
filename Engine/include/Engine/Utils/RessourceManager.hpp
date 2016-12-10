#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Model.hpp>

// Textures and models files extensions loaded in RessourceManager::loadResources
#define TEXTURES_EXT    "png", "jpg", "jpeg"
#define MODELS_EXT      "dae", "obj", "fbx"
#define SOUNDS_EXT      "mp3", "wav"

class RessourceManager
{
private:
    private:
    struct sFile
    {
        std::string content;
        std::string path;
        std::string basename;
    };

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

    Texture&                                        getTexture(const std::string& fileName);
    const std::vector<const char*>&                 getTexturesNames() const;

    std::shared_ptr<Model>                          getModel(const std::string& fileName);
    const std::vector<const char*>&                 getModelsNames() const;

    const std::vector<sSoundStrings>&               getSoundsStrings() const;

    static RessourceManager*                        getInstance();
    static std::string                              getFileExtension(const std::string& fileName);

private:
    std::string                                     getBasename(const std::string& fileName);
    std::string                                     loadFile(const std::string basename, const std::string& fileName);
    Texture&                                        loadTexture(const std::string basename, const std::string& fileName);
    std::shared_ptr<Model>                          loadModel(const std::string basename, const std::string& fileName);

private:
    // Store files content with their basename
    std::unordered_map<std::string, sFile>          _files;

    // Store textures with their basename
    std::unordered_map<std::string, Texture>        _textures;
    // Store textures basename list
    std::vector<const char*>                        _texturesNames;

    // Store models with their basename
    std::unordered_map<std::string, std::shared_ptr<Model>> _models;
    // Store models basename list
    std::vector<const char*>                                _modelsNames;

    // Store sounds basename list
    std::vector<sSoundStrings>                              _soundsStrings;

    //Singleton instance
    static RessourceManager*                        _ressourceManager;
};
