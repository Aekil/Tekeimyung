#include <fstream>
#include <vector>
#include "Utils/RessourceManager.hpp"

RessourceManager*   RessourceManager::_ressourceManager = nullptr;

RessourceManager::RessourceManager() {}

RessourceManager::~RessourceManager() {}

RessourceManager*   RessourceManager::getInstance()
{
    if (!_ressourceManager)
    {
        _ressourceManager = new RessourceManager();
    }

    return _ressourceManager;
}

std::string RessourceManager::getBasename(const std::string& fileName)
{
    size_t basenameOcur;
    std::string basename = fileName;

    basenameOcur = fileName.find_last_of('/');
    if (basenameOcur != std::string::npos)
    {
        basename = fileName.substr(basenameOcur + 1);
    }

    return basename;
}

std::string RessourceManager::getFile(const std::string& fileName)
{
    // Files are stored with their basename
    std::string basename = getBasename(fileName);

    try
    {
        auto &&file = _files.at(basename);
        return file;
    }
    catch(...)
    {
        // The file is not loaded
        return loadFile(basename, fileName);
    }
}

std::string RessourceManager::loadFile(const std::string basename, const std::string& fileName)
{
    std::ifstream               file;
    int                         fileSize;
    std::vector<char>           fileContent;

    file.open(fileName.c_str());
    if (!file.good())
        throw EngineException("Failed to open file " + fileName);

    file.seekg(0, file.end);
    fileSize = file.tellg();
    file.seekg(0, file.beg);

    fileContent.resize(fileSize);
    file.read(fileContent.data(), fileSize);

    _files[basename] = std::string(fileContent.begin(), fileContent.end());
    return _files[basename];
}

Texture&    RessourceManager::getTexture(const std::string& fileName)
{
    // Textures are stored with their basename
    std::string basename = getBasename(fileName);

    try {
        auto &&texture = _textures.at(basename);
        return texture;
    }
    catch(...) {
        // The texture is not loaded
        return loadTexture(basename, fileName);
    }
}

Texture&    RessourceManager::loadTexture(const std::string basename, const std::string& fileName)
{
    _textures[basename] = {};
    _textures[basename].loadFromFile(fileName);

    return _textures[basename];
}