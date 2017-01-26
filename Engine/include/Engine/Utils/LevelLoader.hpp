/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>
#include <unordered_map>
#include <ECS/Entity.hpp>

class LevelLoader
{
 public:
    LevelLoader();
    ~LevelLoader();

    bool save(const std::string& levelName, const std::unordered_map<uint32_t, Entity*>& entities);
};
