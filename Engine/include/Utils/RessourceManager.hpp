#pragma once

#include <string>
#include <unordered_map>
#include "Graphics/Texture.hpp"

class RessourceManager
{
public:
    RessourceManager();
    ~RessourceManager();
    std::string                                     getFile(const std::string& fileName);
    Texture&                                        getTexture(const std::string& fileName);
    static RessourceManager*                        getInstance();
    static std::string                              getFileExtension(const std::string& fileName);

private:
    std::string                                     getBasename(const std::string& fileName);
    std::string                                     loadFile(const std::string basename, const std::string& fileName);
    Texture&                                        loadTexture(const std::string basename, const std::string& fileName);

private:
    // Store files content with their basename
    std::unordered_map<std::string, std::string>     _files;

    // Store textures with their basename
    std::unordered_map<std::string, Texture>        _textures;

    //Singleton instance
    static RessourceManager*                        _ressourceManager;
};
