/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <vector>

#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>
#include <Game/Map/DoubleArray.hpp>

#define LAYER_NUMBER 10

class MapLoader;

class Map final: public DoubleArray<int>
{
friend MapLoader;

public:
    Map() = default;
    ~Map() = default;

    std::unordered_map<int, std::vector<Entity*>>& getMapParts();
    DoubleArray<char>&      getSpawnersPaths();
    DoubleArray<Entity*>&   getEntities();
    const glm::ivec2&       getCastlePos() const;

private:
    void                    init();

private:
    DoubleArray<char>   _spawnersPaths;
    DoubleArray<Entity*> _entities;

    std::unordered_map<int, std::vector<Entity*>> _mapParts;
    glm::ivec2 _castlePos;
};
