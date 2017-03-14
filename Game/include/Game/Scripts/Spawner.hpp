/*
** @Author : Simon AMBROISE
*/

#pragma once

#include    <Engine/Core/ScriptFactory.hpp>
#include <glm/vec3.hpp>

class GameManager;

class       Spawner final : public BaseScript
{
public:
    struct sConfig
    {
        struct sEntity
        {
            std::string name;
            uint32_t spawnAmount{0};
            float   timeUntilNextSpawn{0};

            float elapsedTime{0};
            float spawnedNb{0};
        };
        std::vector<sEntity> spawnableEntities;
        uint32_t associatedWave{0};
    };

public:
    Spawner() = default;
    ~Spawner() = default;

    void    start() override final;
    void    update(float dt) override final;

    bool updateEditor() override final;
    JsonValue saveToJson() override final;
    void loadFromJson(const JsonValue& json) override final;

private:
    void spawnEntity(const std::string& entityName);
    std::vector<glm::vec3> getClosestPath() const;

private:
    Entity* _parent;
    std::vector<sConfig> _configs;

    uint32_t _selectedConfig{0};
    uint32_t _currentWave{0};

private:
    GameManager* _gameManager{nullptr};
    sTransformComponent* _transform;

    std::vector<glm::vec3> _closestPath;
};

REGISTER_SCRIPT(Spawner);
