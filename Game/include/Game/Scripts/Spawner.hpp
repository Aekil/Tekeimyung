/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <ECS/Entity.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Game/Scripts/Path.hpp>

class GameManager;

class       Spawner final : public BaseScript
{
    friend class TutoManager;

public:
    struct sConfig
    {
        struct sEntity
        {
            std::string name;
            uint32_t    spawnAmount = 0;
            float       timeUntilNextSpawn = 0.0f;

            float       elapsedTime = 0.0f;
            uint32_t    amountSpawned = 0;
        };

        std::vector<sEntity>    spawnableEntities;
        uint32_t                associatedWave = 0;

        std::vector<Entity::sHandle>   spawnedEntities;

        //  Remove dead entities from sEntity::spawnedEntities
        void    updateSpawnedEntities();

        bool    areAllEntitiesSpawned();
        bool    areAllSpawnedEntitiesDead();
    };

public:
    Spawner() = default;
    ~Spawner() = default;

    void    start() override final;
    void    update(float dt) override final;

    void    triggerSpawnerConfigs(uint32_t currentWave);
    void    clearSpawnerConfigs();
    bool    isReadyForNextWave();
    void    updateClosestPath();

    //  Json serialization & ImGui edition.
    bool        updateEditor() override final;
    JsonValue   saveToJson() override final;
    void        loadFromJson(const JsonValue& json) override final;

private:
    void        setEnemyPath(Entity* enemy, const std::vector<glm::vec3>& path);
    void        getPath(const glm::ivec2& from, const glm::ivec2& to, std::vector<glm::vec3>& savedPath);
    void        findBlockedPath(glm::ivec2 from, const glm::ivec2& to, std::vector<glm::ivec2>& path, glm::ivec2 closestTileFound);
    void        updateEnemiesPaths();

    void        displayPath(const std::vector<glm::ivec2>& path) const;

    bool        isActive() const;
    void        setActive(bool active);

private:
    //  All the entities will follow the closest path from the Spawner to the Castle
    std::vector<glm::vec3>  getClosestPath() const;

    Entity* spawnEntity(const std::string& entityName);
    void    spawnEntitiesFromConfig(sConfig* waveConfig, float dt);

private:
    Entity*                 _parent;
    std::vector<sConfig>    _configs;

    bool                    _active;
    std::vector<sConfig*>   _currentConfigs;

    GameManager*            _gameManager = nullptr;
    sTransformComponent*    _transform = nullptr;

    Path                    _path;
    std::vector<glm::vec3>  _closestPath;

    // Used to display closest tiles found
    // for path finding debug
    std::vector<glm::ivec2>  _closestTilesFound;

    //  ImGui selection index
    uint32_t                _selectedConfig = 0;
};
