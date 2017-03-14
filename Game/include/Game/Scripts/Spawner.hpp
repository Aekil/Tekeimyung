/*
** @Author : Simon AMBROISE
*/

#pragma once

#include    <Engine/Core/ScriptFactory.hpp>

class       Spawner final : public BaseScript
{
public:
    struct sConfig
    {
        struct sEntity
        {
            std::string name;
            int     spawnAmount{0};
            float   timeUntilNextSpawn{0};
        };
        std::vector<sEntity> spawnableEntities;
        int     associatedWave{0};
    };

public:
    Spawner() = default;
    ~Spawner() = default;

    void    start() override final;
    void    update(float dt) override final;

    bool updateEditor() override final;

public:
    Entity* getWaveManager() const;

    void    setWaveManager(Entity* waveManager);
    void    deleteSpawnerConfig(int index);

private:
    Entity* _parent;
    std::vector<sConfig> _configs;

    uint32_t _selectedConfig{0};

};

REGISTER_SCRIPT(Spawner);
