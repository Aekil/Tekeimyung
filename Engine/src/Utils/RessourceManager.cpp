#include <fstream>
#include <vector>
#include "Utils/RessourceManager.hpp"
#include "Utils/Exception.hpp"

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

std::string  RessourceManager::getFileExtension(const std::string& fileName)
{
    std::size_t size = fileName.rfind('.', fileName.length());
    if (size != std::string::npos)
        return (fileName.substr(size + 1, fileName.length() - size));

    return ("");
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
        return file.content;
    }
    catch(...)
    {
        // The file is not loaded
        return loadFile(basename, fileName);
    }
}

void        RessourceManager::saveFile(const std::string& fileName, const std::string fileContent)
{
    std::ofstream file;
    RessourceManager::sFile fileInfos;
    std::string basename = getBasename(fileName);

    file.open(fileName.c_str(), std::ios::trunc);
    if (!file.good())
        EXCEPT(FileNotFoundException, "Failed to open file \"%s\"", fileName.c_str());

    file << fileContent;
    file.close();

    // The file does not exist is RessourceManager
    if (_files.find(basename) != _files.end())
    {
        // Init file informations
        fileInfos.content = fileContent;
        fileInfos.path = fileName;
        fileInfos.basename = basename;
    }
    else
    {
        // Update file informations
        fileInfos.content = fileContent;
    }
    _files[basename] = fileInfos;
}

std::string RessourceManager::loadFile(const std::string basename, const std::string& fileName)
{
    std::ifstream               file;
    int                         fileSize;
    std::vector<char>           fileContent;
    RessourceManager::sFile     fileInfos;

    file.open(fileName.c_str());
    if (!file.good())
        EXCEPT(FileNotFoundException, "Failed to open file \"%s\"", fileName.c_str());

    file.seekg(0, file.end);
    fileSize = file.tellg();
    file.seekg(0, file.beg);

    fileContent.resize(fileSize);
    file.read(fileContent.data(), fileSize);
    file.close();

    fileInfos.path = fileName;
    fileInfos.content = std::string(fileContent.begin(), fileContent.end());
    fileInfos.basename = basename;
    _files[basename] = fileInfos;
    return _files[basename].content;
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
