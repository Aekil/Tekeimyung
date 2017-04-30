#include <fstream>
#include <sstream>

#include <Game/MapLoader.hpp>
#include <Engine/Debug/Logger.hpp>
#include <Engine/Utils/File.hpp>
#include <Engine/Utils/ResourceManager.hpp>

bool    MapLoader::load(const std::string& mapName, Map& map)
{
    if (!mapName.c_str())
    {
        LOG_ERROR("MapLoader::load: no map specified");
        return (false);
    }

    sLoadInfo loadInfo;
    std::string mapFile = MAPS_DIRECTORY + mapName + ".txt";

    if (!validateMap(mapFile, loadInfo))
    {
        return (false);
    }
    map.allocate(loadInfo.rowsNb, loadInfo.colsNb);

    auto file = ResourceManager::getInstance()->getOrLoadResource<File>(mapFile);
    std::ifstream mapContent(file->getPath());
    if (!mapContent.good())
    {
        LOG_ERROR("MapLoader::validateMap: cannot open map file %s", mapFile.c_str());
        return (false);
    }

    std::string line;
    uint32_t x = 0;
    while (std::getline(mapContent, line))
    {
        if (!line.size())
        {
            continue;
        }

        std::istringstream lineStream(line);
        std::string token;
        uint32_t y = 0;
        while (std::getline(lineStream, token, ','))
        {
            if (!token.size())
            {
                continue;
            }
            std::istringstream tokenStream(token);
            tokenStream >> map[x][y++];
        }
        ++x;
    }

    map.init();
    mapContent.close();
    return (true);
}

bool    MapLoader::validateMap(const std::string& mapFile, sLoadInfo& loadInfo)
{
    auto file = ResourceManager::getInstance()->getOrLoadResource<File>(mapFile);
    std::ifstream mapContent(file->getPath());

    if (!mapContent.good())
    {
        LOG_ERROR("MapLoader::validateMap: cannot open map file %s", mapFile.c_str());
        return (false);
    }

    std::string line;
    while (std::getline(mapContent, line))
    {
        if (!line.size())
        {
            continue;
        }
        std::istringstream lineStream(line);
        std::string token;
        uint32_t colsNb = 0;
        while (std::getline(lineStream, token, ','))
        {
            if (!token.size())
            {
                continue;
            }
            ++colsNb;
        }
        if (colsNb != loadInfo.colsNb && // Not the same number of columns as previous row
            loadInfo.colsNb != -1) // First row, don't check
        {
            LOG_ERROR("MapLoader::validateMap: 2 rows have not the same columns number: %d:%d and %d:%d",
                    loadInfo.rowsNb - 1,
                    loadInfo.colsNb,
                    loadInfo.rowsNb,
                    colsNb);
            mapContent.close();
            return (false);
        }
        loadInfo.colsNb = colsNb;

        // Don't count lines with no token
        if (colsNb)
        {
            ++loadInfo.rowsNb;
        }
    }

    if (loadInfo.colsNb == -1 ||
        loadInfo.rowsNb == 0)
    {
        LOG_ERROR("MapLoader::validateMap: Map is empty");
        mapContent.close();
        return (false);
    }

    mapContent.close();
    return (true);
}
