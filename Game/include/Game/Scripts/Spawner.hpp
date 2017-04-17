/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Core/ScriptFactory.hpp>
#include <Game/Scripts/Path.hpp>

class GameManager;

class       Spawner final : public BaseScript
{
public:
    struct sConfig
    {
        struct sEntity
        {
            std::string name;
            uint32_t spawnAmount{ 0 };
            float   timeUntilNextSpawn{ 0 };

            float elapsedTime{ 0 };
            float spawnedNb{ 0 };
        };
        std::vector<sEntity> spawnableEntities;
        uint32_t associatedWave{ 0 };

        std::vector<uint32_t> spawnedEntities;

        void updateSpawnedEntities(); // Remove dead entities from sEntity::spawnedEntities
        bool allEntitiesSpawned();
    };

public:
    Spawner() = default;
    ~Spawner() = default;

    void    start() override final;
    void    update(float dt) override final;

    bool updateEditor() override final;
    JsonValue saveToJson() override final;
    void loadFromJson(const JsonValue& json) override final;

    void startWave(uint32_t waveNb);
    bool checkEndWave();

private:
    std::vector<glm::vec3> getClosestPath();
    Entity* spawnEntity(const std::string& entityName);
    void spawnEntities(sConfig* waveConfig, float dt);

private:
    Entity* _parent;
    std::vector<sConfig> _configs;

    uint32_t _selectedConfig{ 0 };

    std::vector<sConfig*> _currentWaves;

    GameManager* _gameManager{ nullptr };
    sTransformComponent* _transform;

    std::vector<glm::vec3> _closestPath;

    Path                    _path;
};

REGISTER_SCRIPT(Spawner);
