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
    _progressBar.maxProgress = 5.0f;
    _progressBar.currentProgress = 0.0f;
    _progressBar.init("TIMER_BAR_EMPTY", "TIMER_BAR");
    _progressBar.display(false);
}

void        WaveManager::update(float dt)
{
    if (_currentWave >= _waves)
        return;

    if (_progressBar.currentProgress > 0.0f ||
        _currentWave == -1)
    {
        _progressBar.currentProgress -= dt;
        _progressBar.update();

        if (_progressBar.currentProgress <= 0.0f)
        {
            handleStartWave();
        }
        return;
    }

    // All spawners entities are dead
    if (checkEndWave())
    {
        if (_currentWave + 1 < _waves)
        {
            handleEndWave();
        }
    }
}

bool    WaveManager::isWaiting()
{
    return (_waiting);
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

void    WaveManager::handleStartWave()
{
    auto em = EntityFactory::getBindedEntityManager();

    _progressBar.display(false);
    ++_currentWave;
    if (_currentWave >= _waves)
        return;
    startWave(_currentWave);
    _waiting = false;

    Entity* player = em->getEntityByTag("Player");
    if (!player)
    {
        LOG_WARN("Can't find entity with tag Player");
        return;
    }
    sTransformComponent* playerTransform = player->getComponent<sTransformComponent>();
    playerTransform->setPos({96.0f, 22.5f, 116.0f});
}

void    WaveManager::handleEndWave()
{
    auto em = EntityFactory::getBindedEntityManager();
    _progressBar.currentProgress = _progressBar.maxProgress;
    _progressBar.display(true);
    _waiting = true;

    Entity* player = em->getEntityByTag("Player");
    if (!player)
    {
        LOG_WARN("Can't find entity with tag Player");
        return;
    }
    sTransformComponent* playerTransform = player->getComponent<sTransformComponent>();
    playerTransform->setPos({65.0f, 16.250f, 33.0f});
}
