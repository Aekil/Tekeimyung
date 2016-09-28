#pragma once

#include <string>
#include <unordered_map>
#include "EngineException.hpp"

class RessourceManager
{
public:
    RessourceManager();
    ~RessourceManager();
    std::string                                     getFile(const std::string& fileName);
    static RessourceManager*                        getInstance();

private:
    std::string                                     getBasename(const std::string& fileName);
    std::string                                     loadFile(const std::string basename, const std::string& fileName);

private:
    // Store files content with their basename
    std::unordered_map<std::string, std::string>     _files;

    //Singleton instance
    static RessourceManager*                        _ressourceManager;
};  