/*
** Author : Mathieu CHASSARA
*/

#include <limits>

#include <Engine/EntityFactory.hpp>
#include <Engine/Utils/LevelLoader.hpp>

#include <Game/GameStates/VictoryScreenState.hpp>
#include <Game/GameStates/DefeatScreenState.hpp>
#include <Game/Manager/GameManager.hpp>
#include <Game/Scripts/Spawner.hpp>
#include <Game/Manager/TutoManager.hpp>
#include <Game/Manager/WaveManager.hpp>
#include <Game/Character/Enemy.hpp>

void        WaveManager::start()
{
    auto    em = EntityFactory::getBindedEntityManager();
    this->_state = WaveManager::eState::STARTING;
    this->_boardState = WaveManager::eBoardState::IDLE;
    //this->_progressBar.currentProgress = 0.0f;
    //this->_progressBar.maxProgress = 5.0f;
    //this->_progressBar.init("TIMER_BAR_EMPTY", "TIMER_BAR");
    //this->_progressBar.display(false);
    this->_mapParts.resize(this->_waves);
    this->retrieveGoldManager();
}

void        WaveManager::update(float dt)
{
    switch (this->_state)
    {
    case eState::STARTING:
        LOG_DEBUG("WaveManager's state: %s", "STARTING");
        if (this->_tutorialIsFinished)
        {
            this->_goldManager->setGolds(STARTING_AMOUNT_OF_GOLD);
            this->_state = eState::PENDING_WAVE;
        }
        break;
    case eState::PENDING_WAVE:
        //this->updateProgressBar(dt);
        //if (this->_progressBar.currentProgress <= 0.0f)
        //{
        //    this->_progressBar.display(false);
        //    this->startWave(this->_currentWave + 1);
        //    this->_state = eState::ONGOING_WAVE;
        //    LOG_DEBUG("WaveManager's state: %s", "ONGOING_WAVE");
        //}
        if (this->keyboard.getStateMap()[Keyboard::eKey::SPACE] == Keyboard::eKeyState::KEY_RELEASED)
        {
            handlePendingWave();
        }
        break;
    case eState::ONGOING_WAVE:
        if (this->checkBoardState(dt) == true)
        {
            this->_state = eState::PENDING_WAVE;
            this->_goldManager->setIncreaseOnTime(false);
            TutoManager::display(true);
            //this->_progressBar.currentProgress = this->_progressBar.maxProgress;
            //this->_progressBar.display(true);
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

void            WaveManager::retrieveGoldManager()
{
    auto        em = EntityFactory::getBindedEntityManager();
    Entity*     goldManager = em->getEntityByTag("GameManager");

    if (goldManager != nullptr)
    {
        sScriptComponent*   scriptComponent = goldManager->getComponent<sScriptComponent>();

        if (scriptComponent != nullptr)
        {
            GoldManager*    goldScript = scriptComponent->getScript<GoldManager>("GoldManager");

            if (goldScript != nullptr)
                this->_goldManager = goldScript;
        }
    }
}

void            WaveManager::startWave(uint32_t wave)
{
    auto        em = EntityFactory::getBindedEntityManager();
    const auto& spawners = em->getEntitiesByTag("Spawner");

    this->_currentWave = wave;
    if (this->_mapParts[this->_currentWave - 1] != -1)
        this->spawnMapPart();
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

        if (scriptComponent->enabled == true)
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



void        WaveManager::spawnMapPart()
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* gameManager = em->getEntityByTag("GameManager");

    if (!gameManager)
    {
        LOG_WARN("Can't find entity with GameManager tag");
        return;
    }

    auto    scriptComponent = gameManager->getComponent<sScriptComponent>();

    if (!scriptComponent)
    {
        LOG_WARN("Can't find scriptComponent on GameManager entity");
        return;
    }

    auto    gameManagerScript = scriptComponent->getScript<GameManager>("GameManager");

    gameManagerScript->displayMapParts(this->_mapParts[this->_currentWave - 1]);
}



bool            WaveManager::checkEndWave()
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

        if (scriptComponent->enabled == false)
            continue;

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

    return (em->getEntityByTag("Castle") == nullptr &&
        em->getEntityByTag("CastleExplosion") == nullptr);
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

void        WaveManager::handleEndWave()
{
    auto    em = EntityFactory::getBindedEntityManager();

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
            Destroy(enemy);
            continue;
        }

        Enemy*  enemyScript = scriptComponent->getScript<Enemy>("Enemy");

        if (!enemyScript)
        {
            Destroy(enemy);
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

    ImGui::BeginGroup();
    ImGui::Text("Wave/MapPart");
    for (int idx = 0; idx < this->_mapParts.size(); idx++)
    {
        ImGui::PushID(idx);
        ImGui::InputInt("Map Part", &(this->_mapParts[idx]));
        ImGui::PopID();
    }
    ImGui::EndGroup();

    return (changed);
}

JsonValue                   WaveManager::saveToJson()
{
    JsonValue               waveManagerJson;
    std::vector<JsonValue>  configsJson;

    for (auto& mapPart : this->_mapParts)
    {
        JsonValue   configJson;

        configJson.setInt("map_part", mapPart);
        configsJson.push_back(configJson);
    }
    waveManagerJson.setValueVec("map_parts", configsJson);
    waveManagerJson.setUInt("waves_max", this->_waves);
    return (waveManagerJson);
}



void                WaveManager::loadFromJson(const JsonValue& json)
{
    unsigned int    wavesMax = json.getUInt("waves_max", 0);
    auto            mapParts = json.get()["map_parts"];
    int             idx = 0;

    this->_waves = wavesMax;
    this->_mapParts.resize(this->_waves);

    if (mapParts.size() > 0 && mapParts.type() != Json::ValueType::arrayValue)
    {
        LOG_ERROR("WaveManager::loadFromJson error: configs is not an array");
        return;
    }

    for (const auto& mapPart : mapParts)
    {
        JsonValue   value(mapPart);

        this->_mapParts[idx++] = value.getInt("map_part", -1);
    }
}



void    WaveManager::updateProgressBar(float deltaTime)
{
    this->_progressBar.currentProgress -= deltaTime;
    this->_progressBar.currentProgress = std::max(0.0f, this->_progressBar.currentProgress);
    this->_progressBar.update();
}

void    WaveManager::setTutorialIsFinished(bool tutorialIsFinished)
{
    this->_tutorialIsFinished = tutorialIsFinished;
}

void    WaveManager::handlePendingWave()
{
    this->startWave(this->_currentWave + 1);
    this->_state = eState::ONGOING_WAVE;
    this->_goldManager->setIncreaseOnTime(true);
    TutoManager::display(false);
    LOG_DEBUG("WaveManager's state: %s", "ONGOING_WAVE");
}
