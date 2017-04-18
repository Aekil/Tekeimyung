/*
** Author : Mathieu CHASSARA
*/

#include <limits>

#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/LevelLoader.hpp>

#include <Game/GameStates/VictoryScreenState.hpp>
#include <Game/GameStates/DefeatScreenState.hpp>
#include <Game/Scripts/Build.hpp>
#include <Game/Scripts/Spawner.hpp>
#include <Game/Scripts/WaveManager.hpp>
#include <Game/Scripts/Enemy.hpp>

void        WaveManager::start()
{
    auto    em = EntityFactory::getBindedEntityManager();

    this->_state = WaveManager::eState::STARTING;
    this->_boardState = WaveManager::eBoardState::IDLE;
    this->_progressBar.currentProgress = 0.0f;
    this->_progressBar.maxProgress = 5.0f;
    this->_progressBar.init("TIMER_BAR_EMPTY", "TIMER_BAR");
    this->_progressBar.display(false);
}

void        WaveManager::update(float dt)
{
    switch (this->_state)
    {
        case eState::STARTING:
            LOG_DEBUG("WaveManager's state: %s", "STARTING");
            this->_state = eState::PENDING_WAVE;
            break;
        case eState::PENDING_WAVE:
            this->updateProgressBar(dt);
            if (this->_progressBar.currentProgress <= 0.0f)
            {
                this->_progressBar.display(false);
                //this->updatePlayerState(true, 1);
                this->startWave(this->_currentWave + 1);
                this->_state = eState::ONGOING_WAVE;
                LOG_DEBUG("WaveManager's state: %s", "ONGOING_WAVE");
            }
            break;
        case eState::ONGOING_WAVE:
            if (this->checkBoardState(dt) == true)
            {
                this->_state = eState::PENDING_WAVE;
                //this->updatePlayerState(false, 0);
                this->_progressBar.currentProgress = this->_progressBar.maxProgress;
                this->_progressBar.display(true);
                LOG_DEBUG("WaveManager's state: %s", "PENDING_WAVE");
            }
            if (this->_boardState != eBoardState::IDLE)
                this->_state = eState::ENDING;
            break;
        case eState::ENDING:
            LOG_DEBUG("WaveManager's state: %s", "ENDING");
            this->_state = eState::ENDED;
            this->end();
            break;
        case eState::ENDED:
            LOG_DEBUG("WaveManager's state: %s", "ENDED");
            break;
    }
}

int     WaveManager::getCurrentWave() const
{
    return (_currentWave);
}

int     WaveManager::getNbWaves() const
{
    return (_waves);
}

WaveManager::eState WaveManager::getManagerState() const
{
    return (this->_state);
}

void            WaveManager::startWave(uint32_t wave)
{
    auto        em = EntityFactory::getBindedEntityManager();
    const auto& spawners = em->getEntitiesByTag("Spawner");

    this->_currentWave = wave;
    for (auto &spawner : spawners)
    {
        auto    scriptComponent = spawner->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on Spawner entity");
            return;
        }

        auto    spawnerScript = scriptComponent->getScript<Spawner>("Spawner");
        
        if (!spawnerScript)
        {
            LOG_WARN("Entities with tag \"Spawner\" should have a \"Spawner\" script attached");
            return;
        }

        spawnerScript->triggerSpawnerConfigs(this->_currentWave);
    }
}

bool    WaveManager::checkBoardState(float deltaTime)
{
    if (this->isGameOver() == true)
    {
        this->_boardState = eBoardState::DEFEAT;
        return (false);
    }

    return (this->checkEndWave());
}

bool    WaveManager::checkEndWave()
{
    auto        em = EntityFactory::getBindedEntityManager();
    const auto& spawners = em->getEntitiesByTag("Spawner");

    for (auto &spawner : spawners)
    {
        auto    scriptComponent = spawner->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find sScriptComponent on Spawner entity");
            continue;
        }

        auto    spawnerScript = scriptComponent->getScript<Spawner>("Spawner");

        if (spawnerScript->isReadyForNextWave() == false)
            return (false);
        else
            spawnerScript->clearSpawnerConfigs();
    }

    if (this->_currentWave == this->_waves)
        this->_boardState = eBoardState::VICTORY;
    return (true);
}

bool        WaveManager::isGameOver()
{
    auto    em = EntityFactory::getBindedEntityManager();

    return (em->getEntityByTag("Castle") == nullptr);
}

void        WaveManager::end()
{
    auto    gameStateManager = GameWindow::getInstance()->getGameStateManager();

    switch (this->_boardState)
    {
        case eBoardState::IDLE:
        case eBoardState::VICTORY:
            gameStateManager->addState<VictoryScreenState>();
            break;
        case eBoardState::DEFEAT:
            this->handleGameOver();
            gameStateManager->addState<DefeatScreenState>();
            break;
        default:
            LOG_WARN("The WaveManager should not end without VICTORY or DEFEAT board state !");
            break;
    }
}

//void        WaveManager::updatePlayerState(bool teleport, unsigned int layer)
//{
//    auto    em = EntityFactory::getBindedEntityManager();
//    Entity* player = em->getEntityByTag("Player");
//
//    if (!player)
//    {
//        LOG_WARN("Could not find entity with tag \"%s\"", "Player");
//        return;
//    }
//
//    if (teleport == true)
//    {
//        sTransformComponent*    playerTransform = player->getComponent<sTransformComponent>();
//
//        playerTransform->setPos({ 96.0f, 22.5f, 116.0f });
//    }
//
//    auto    scriptComponent = player->getComponent<sScriptComponent>();
//
//    if (!scriptComponent)
//    {
//        LOG_WARN("Could not find sScriptComponent on entity \"%s\"", player->getComponent<sNameComponent>()->value.c_str());
//        return;
//    }
//
//    Build*  buildScript = scriptComponent->getScript<Build>("Build");
//
//    if (buildScript == nullptr)
//    {
//        LOG_WARN("Could not find \"Build\" script on entity \"%s\"", player->getComponent<sNameComponent>()->value.c_str());
//        return;
//    }
//    
//    buildScript->setLayer(layer);
//}

void    WaveManager::handleEndWave()
{
    auto em = EntityFactory::getBindedEntityManager();
    _progressBar.currentProgress = _progressBar.maxProgress;
    _progressBar.display(true);
}

void        WaveManager::handleGameOver()
{
    auto    em = EntityFactory::getBindedEntityManager();

    //  Game over, destroy all enemies
    const auto& enemies = em->getEntitiesByTag("Enemy");

    for (auto& enemy : enemies)
    {
        auto    scriptComponent = enemy->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            em->destroyEntityRegister(enemy);
            continue;
        }

        Enemy*  enemyScript = scriptComponent->getScript<Enemy>("Enemy");

        if (!enemyScript)
        {
            em->destroyEntityRegister(enemy);
            continue;
        }

        enemyScript->death();
    }
}

bool            WaveManager::updateEditor()
{
    bool        changed = false;
    int         wavesMax = (int) this->_waves;


    ImGui::BeginGroup();
    if (ImGui::InputInt("Number of waves", &wavesMax, 1, 100, ImGuiInputTextFlags_CharsNoBlank))
    {
        wavesMax = wavesMax < 0 ? 0 : wavesMax;
        wavesMax = wavesMax > std::numeric_limits<unsigned int>::max() ? std::numeric_limits<unsigned int>::max() : wavesMax;
        this->_waves = wavesMax;
        changed |= true;
    }
    ImGui::EndGroup();

    return (changed);
}

JsonValue       WaveManager::saveToJson()
{
    JsonValue   waveManagerJson;

    waveManagerJson.setUInt("waves_max", this->_waves);
    return (waveManagerJson);
}

void                WaveManager::loadFromJson(const JsonValue& json)
{
    unsigned int    wavesMax = json.getUInt("waves_max", 0);

    this->_waves = wavesMax;
}

void    WaveManager::updateProgressBar(float deltaTime)
{
    this->_progressBar.currentProgress -= deltaTime;
    this->_progressBar.currentProgress = std::max(0.0f, this->_progressBar.currentProgress);
    this->_progressBar.update();
}