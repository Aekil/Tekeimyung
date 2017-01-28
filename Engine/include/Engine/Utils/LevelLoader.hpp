/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>
#include <Engine/Utils/JsonValue.hpp>

#define LEVELS_DIRECTORY    "resources/levels/"

class LevelLoader
{
 public:
    LevelLoader();
    ~LevelLoader();

    static LevelLoader*                 getInstance();

    const std::vector<std::string>&     getLevels() const;
    std::vector<std::string>&           getLevels();

    void                                save(const std::string& levelName, const std::unordered_map<uint32_t, Entity*>& entities);
    void                                load(const std::string& levelName, EntityManager* em);
    void                                addComponentToEntity(Entity* entity, const std::string& entityTypeName, const JsonValue& componentJson, const std::string& componentName);

    void                                loadDirectory(const std::string& directory);

private:
    std::vector<std::string>            _levels;

    static std::unique_ptr<LevelLoader> _instance;
};
