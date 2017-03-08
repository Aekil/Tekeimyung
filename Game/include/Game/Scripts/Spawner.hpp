/*
** @Author : Simon AMBROISE
*/

#pragma once

#include    <Engine/Core/ScriptFactory.hpp>

#include    <Game/Scripts/SpawnerConfiguration.hpp>

class       Spawner final : public BaseScript
{
public:
    Spawner() = default;
    ~Spawner() = default;

    void    start() override final;
    void    update(float dt) override final;

private:
    Entity* _parent;
    std::vector<SpawnerConfiguration>   _configs;

public:
    Entity* getWaveManager() const;
    std::vector<SpawnerConfiguration>   getSpawnerConfigs() const;

    void    setWaveManager(Entity* waveManager);
    void    addSpawnerConfig(SpawnerConfiguration config);
    void    deleteSpawnerConfig(int index);
};

REGISTER_SCRIPT(Spawner);
