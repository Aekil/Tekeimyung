/*
** Author : Mathieu CHASSARA
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/Spawner.hpp>
#include <Game/Scripts/WaveManager.hpp>

void        WaveManager::start()
{
    auto em = EntityFactory::getBindedEntityManager();
    this->_waves = 4;
    this->_timeBeforeWaveStarts = 5.0f;
}

void        WaveManager::update(float dt)
{
    if (_currentWave >= _waves)
        return;

    // All spawners entities are dead
    if (checkEndWave())
    {
        ++_currentWave;
        if (_currentWave >= _waves)
            return;
        startWave(_currentWave);
    }
}

int         WaveManager::getWaves() const
{
    return (this->_waves);
}

float       WaveManager::getTimeBeforeWaveStarts() const
{
    return (this->_timeBeforeWaveStarts);
}

void        WaveManager::setWaves(int waves)
{
    this->_waves = waves;
}

void        WaveManager::setTimeBeforeWaveStarts(float time)
{
    this->_timeBeforeWaveStarts = time;
}

void    WaveManager::startWave(uint32_t wave)
{
    auto em = EntityFactory::getBindedEntityManager();
    const auto& spawners = em->getEntitiesByTag("Spawner");
    LOG_INFO("START WAVE %d", (int)wave);

    for (auto &spawner : spawners)
    {
        auto scriptComponent = spawner->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on Spawner entity");
            return;
        }

        auto spawnerScript = scriptComponent->getScript<Spawner>("Spawner");
        spawnerScript->startWave(wave);
    }
}

bool    WaveManager::checkEndWave()
{
    auto em = EntityFactory::getBindedEntityManager();
    const auto& spawners = em->getEntitiesByTag("Spawner");
    bool endWave = true;

    for (auto &spawner : spawners)
    {
        auto scriptComponent = spawner->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on Spawner entity");
            continue;
        }

        auto spawnerScript = scriptComponent->getScript<Spawner>("Spawner");
        if (!spawnerScript->checkEndWave())
        {
            endWave = false;
        }
    }

    return (endWave);
}
