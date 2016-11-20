#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Model.hpp>

// Textures and models files extensions loaded in RessourceManager::loadResources
#define TEXTURES_EXT "png", "jpg", "jpeg"
#define MODELS_EXT "dae", "obj", "fbx"

class RessourceManager
{
public:
    RessourceManager();
    ~RessourceManager();

    void                                            loadResources(const std::string& directory);

    std::string                                     getFile(const std::string& fileName);
    void                                            saveFile(const std::string& fileName, const std::string fileContent);
    Texture&                                        getTexture(const std::string& fileName);
    const std::vector<const char*>&                 getTexturesNames() const;
    std::shared_ptr<Model>                          getModel(const std::string& fileName);
    const std::vector<const char*>&                 getModelsNames() const;

    static RessourceManager*                        getInstance();
    static std::string                              getFileExtension(const std::string& fileName);

private:
    struct sFile
    {
        std::string content;
        std::string path;
        std::string basename;
    };

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

    //Singleton instance
    static RessourceManager*                        _ressourceManager;
};
