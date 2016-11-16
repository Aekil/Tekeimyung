#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Model.hpp>

class RessourceManager
{
public:
    RessourceManager();
    ~RessourceManager();

    std::string                                     getFile(const std::string& fileName);
    void                                            saveFile(const std::string& fileName, const std::string fileContent);
    Texture&                                        getTexture(const std::string& fileName);
    std::shared_ptr<Model>                          getModel(const std::string& fileName);

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

    // Store models with their basename
    std::unordered_map<std::string, std::shared_ptr<Model>> _models;

    //Singleton instance
    static RessourceManager*                        _ressourceManager;
};
