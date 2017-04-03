/*
** Author : Mathieu CHASSARA
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/LevelLoader.hpp>

#include <Game/Scripts/Build.hpp>
#include <Game/Scripts/Spawner.hpp>
#include <Game/Scripts/WaveManager.hpp>
#include <Game/Scripts/Enemy.hpp>

void        WaveManager::start()
{
    auto em = EntityFactory::getBindedEntityManager();
    this->_waves = 12;
    _progressBar.maxProgress = 15.0f;
    _progressBar.currentProgress = 0.0f;
    _progressBar.init("TIMER_BAR_EMPTY", "TIMER_BAR");
    _progressBar.display(false);

    // Get Player
    {
        auto em = EntityFactory::getBindedEntityManager();
        Entity* player = em->getEntityByTag("Player");
        if (!player)
        {
            LOG_WARN("Can't find entity with Player tag");
            return;
        }

        auto scriptComponent = player->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on Player entity");
            return;
        }

        _playerBuild = scriptComponent->getScript<Build>("Build");
    }
}

void        WaveManager::update(float dt)
{
    if (_currentWave >= _waves)
        return;

    if (checkGameOver())
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
        else
        {
            auto    gameStateManager = GameWindow::getInstance()->getGameStateManager();
            auto    defeatState = LevelLoader::getInstance()->createLevelState("WinScreen", gameStateManager);

            gameStateManager->addState(defeatState);
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

bool    WaveManager::checkGameOver()
{
    auto em = EntityFactory::getBindedEntityManager();
    if (em->getEntityByTag("Castle"))
        return (false);

    handleGameOver();

    return (true);
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
}

void    WaveManager::handleEndWave()
{
    auto em = EntityFactory::getBindedEntityManager();
    _progressBar.currentProgress = _progressBar.maxProgress;
    _progressBar.display(true);
    _waiting = true;
}

void    WaveManager::handleGameOver()
{
    auto em = EntityFactory::getBindedEntityManager();

    // Game over, destroy all enemies
    const auto& enemies = em->getEntitiesByTag("Enemy");
    for (auto& enemy: enemies)
    {
        auto scriptComponent = enemy->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            em->destroyEntityRegister(enemy);
            continue;
        }

        Enemy* enemyScript = scriptComponent->getScript<Enemy>("Enemy");
        if (!enemyScript)
        {
            em->destroyEntityRegister(enemy);
            continue;
        }

        enemyScript->death();
    }

    _currentWave = _waves;
    _progressBar.display(false);
}
