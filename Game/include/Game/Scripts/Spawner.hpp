/*
** @Author : Simon AMBROISE
*/

#pragma once

#include    <Engine/Core/ScriptFactory.hpp>
#include    <glm/vec3.hpp>

class GameManager;

class       Spawner final : public BaseScript
{
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

        std::vector<uint32_t>   spawnedEntities;

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

    //  Json serialization & ImGui edition.
    bool        updateEditor() override final;
    JsonValue   saveToJson() override final;
    void        loadFromJson(const JsonValue& json) override final;

    bool        isActive() const;
    void        setActive(bool active);

    void        triggerSpawnerConfigs(uint32_t currentWave);
    void        clearSpawnerConfigs();
    bool        isReadyForNextWave();

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

    std::vector<glm::vec3>  _closestPath;

    //  ImGui selection index
    uint32_t                _selectedConfig = 0;
};

REGISTER_SCRIPT(Spawner);
