/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <string>

#include <Game/Map.hpp>

#define MAPS_DIRECTORY    "resources/maps/"

class MapLoader
{
private:
    struct sLoadInfo
    {
        int colsNb = -1;
        int rowsNb = 0;
    };

public:
    MapLoader() = default;
    ~MapLoader() = default;

    static bool        load(const std::string& mapName, Map& map);

private:
    static bool         validateMap(const std::string& mapFile, sLoadInfo& loadInfo);
};
