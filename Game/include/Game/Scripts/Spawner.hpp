/*
** @Author : Simon AMBROISE
*/

#pragma once

<<<<<<< HEAD
#include <glm/vec3.hpp>
=======
#include    <Engine/Core/ScriptFactory.hpp>
#include    <glm/vec3.hpp>
>>>>>>> feature-wave-manager

#include <Engine/Core/ScriptFactory.hpp>
#include <Game/Scripts/Path.hpp>

#define CASTLE_POS 8, 0

class GameManager;

class       Spawner final : public BaseScript
{
public:
    struct sConfig
    {
        struct sEntity
        {
            std::string name;
<<<<<<< HEAD
            uint32_t spawnAmount{ 0 };
            float   timeUntilNextSpawn{ 0 };

            float elapsedTime{ 0 };
            float spawnedNb{ 0 };
        };
        std::vector<sEntity> spawnableEntities;
        uint32_t associatedWave{ 0 };
=======
            uint32_t    spawnAmount = 0;
            float       timeUntilNextSpawn = 0.0f;

            float       elapsedTime = 0.0f;
            uint32_t    amountSpawned = 0;
        };
>>>>>>> feature-wave-manager

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

<<<<<<< HEAD
    void startWave(uint32_t waveNb);
    bool checkEndWave();
    void updateClosestPath();

private:
    void setEnemyPath(Entity* enemy, const std::vector<glm::vec3>& path);
    void getPath(const glm::ivec2& from, const glm::ivec2& to, std::vector<glm::vec3>& savedPath);
    void updateEnemiesPaths();
=======
    bool        isActive() const;
    void        setActive(bool active);

    void        triggerSpawnerConfigs(uint32_t currentWave);
    void        clearSpawnerConfigs();
    bool        isReadyForNextWave();

private:
    //  All the entities will follow the closest path from the Spawner to the Castle
    std::vector<glm::vec3>  getClosestPath() const;

>>>>>>> feature-wave-manager
    Entity* spawnEntity(const std::string& entityName);
    void    spawnEntitiesFromConfig(sConfig* waveConfig, float dt);

private:
    Entity*                 _parent;
    std::vector<sConfig>    _configs;

<<<<<<< HEAD
    uint32_t _selectedConfig{ 0 };
=======
    bool                    _active;
    std::vector<sConfig*>   _currentConfigs;
>>>>>>> feature-wave-manager

    GameManager*            _gameManager = nullptr;
    sTransformComponent*    _transform = nullptr;

<<<<<<< HEAD
    GameManager* _gameManager{ nullptr };
    sTransformComponent* _transform;

    std::vector<glm::vec3> _closestPath;

    Path                    _path;
=======
    std::vector<glm::vec3>  _closestPath;

    //  ImGui selection index
    uint32_t                _selectedConfig = 0;
>>>>>>> feature-wave-manager
};

REGISTER_SCRIPT(Spawner);
