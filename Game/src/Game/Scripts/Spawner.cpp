/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Game/Character/Enemy.hpp>
#include <Game/Manager/GameManager.hpp>
#include <Game/Scripts/Path.hpp>
#include <Game/Scripts/Spawner.hpp>
#include <Game/Manager/WaveManager.hpp>

void Spawner::start()
{
    auto    em = EntityFactory::getBindedEntityManager();

    this->_transform = this->getComponent<sTransformComponent>();

    // Init closest path
    {
        Entity* gameManager = em->getEntityByTag("GameManager");
        if (!gameManager)
        {
            LOG_WARN("Could not find entity with tag \"%s\"", "GameManager");
            return;
        }

        auto    scriptComponent = gameManager->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Could not find %s on GameManager entity", "sScriptComponent");
            return;
        }

        this->_gameManager = scriptComponent->getScript<GameManager>("GameManager");
        this->updateClosestPath();
    }
}

/**
    This function updates the Spawner every tick.
    It updates each config currently hooked to the Spawner
    and checks if all the configs have finished spawning their entities.

    If yes, the Spawner is set as inactive and can no longer trigger the configs.
    If no, the Spawner continues to update its configs.
*/
void    Spawner::update(float deltaTime)
{

        auto&   it = this->_currentConfigs.begin();

        for (it; it != this->_currentConfigs.end(); ++it)
        {
            Spawner::sConfig*   waveConfig = *it;

            waveConfig->updateSpawnedEntities();
            if (this->isActive() == true)
            {
                bool    hasFinished = true;
                if (waveConfig->areAllEntitiesSpawned() == false)
                {
                    this->spawnEntitiesFromConfig(waveConfig, deltaTime);
                    hasFinished = false;
                }
                this->setActive(!hasFinished);
            }
        }
}

bool Spawner::updateEditor()
{
    bool changed = false;

    ImGui::Text("Waves configs");
    // Create new config
    {
        if (ImGui::Button("New config"))
        {
            _configs.push_back(sConfig());
        }
    }


    // Configs list
    {
        uint32_t i = 0;
        ImGui::BeginChild("Waves configs", ImVec2(0, 100), true);
        for (auto& config: _configs)
        {
            ImGui::PushID(i);
            if (ImGui::Selectable("config", _selectedConfig == i))
            {
                _selectedConfig = i;
            }
            ImGui::PopID();
            i++;
        }
        ImGui::EndChild();
    }


    if (_configs.size() == 0)
        return (false);

    // Configs editor
    {
        auto& config = _configs[_selectedConfig];
        static std::vector<const char*> spawnableEntityTypes = {"ENEMY", "ENEMY_SPEED", "ENEMY_TANK"};

        int wave = config.associatedWave;
        if (ImGui::InputInt("Associated wave", &wave, 1, ImGuiInputTextFlags_AllowTabInput))
        {
            if (wave < 0)
                wave = 0;
            config.associatedWave = wave;
            changed = true;
        }

        // Add spawnable entity
        {
            if (ImGui::Button("New entity"))
            {
                sConfig::sEntity entity;
                entity.name = spawnableEntityTypes[0];
                config.spawnableEntities.push_back(entity);
            }
        }


        uint32_t i = 0;
        ImGui::Text("Spawnable entities");
        for (auto& entity: config.spawnableEntities)
        {
            ImGui::PushID(i);
            ImGui::Text("--------------------\n");
            Helper::updateComboString("type", spawnableEntityTypes, entity.name);
            int amount = entity.spawnAmount;
            if (ImGui::InputInt("Amount", &amount, 5, ImGuiInputTextFlags_AllowTabInput))
            {
                if (amount < 0)
                    amount = 0;
                entity.spawnAmount = amount;
                changed = true;
            }
            changed |= ImGui::InputFloat("Spawn time", &entity.timeUntilNextSpawn, 0.5f, ImGuiInputTextFlags_AllowTabInput);
            ImGui::PopID();
            i++;
        }
    }

    return (changed);
}

JsonValue Spawner::saveToJson()
{
    JsonValue json;
    std::vector<JsonValue> configsJson;

    for (auto& config: _configs)
    {
        JsonValue configJson;
        std::vector<JsonValue> entitiesJson;

        for (auto& entity: config.spawnableEntities)
        {
            JsonValue entityJson;
            entityJson.setString("type", entity.name);
            entityJson.setUInt("amount", entity.spawnAmount);
            entityJson.setFloat("spawn_time", entity.timeUntilNextSpawn);
            entitiesJson.push_back(entityJson);
        }
        configJson.setUInt("associated_wave", config.associatedWave);
        configJson.setValueVec("spawnable_entities", entitiesJson);
        configsJson.push_back(configJson);
    }

    json.setValueVec("configs", configsJson);
    return (json);
}

void    Spawner::loadFromJson(const JsonValue& json)
{
    auto configs = json.get()["configs"];
    if (configs.size() > 0 && configs.type() != Json::ValueType::arrayValue)
    {
        LOG_ERROR("Spawner::loadFromJson error: configs is not an array");
        return;
    }

    for (const auto& config_: configs)
    {
        // Load animation
        sConfig config;
        JsonValue configJson(config_);

        config.associatedWave = configJson.getUInt("associated_wave", 0);
        auto entities = configJson.get()["spawnable_entities"];
        if (entities.size() > 0 && entities.type() != Json::ValueType::arrayValue)
        {
            LOG_ERROR("Spawner::loadFromJson error: entities is not an array");
            continue;
        }

        for (auto& entity_: entities)
        {
            JsonValue entityJson(entity_);
            sConfig::sEntity entity;

            entity.name = entityJson.getString("type", "ENEMY");
            entity.spawnAmount = entityJson.getUInt("amount", 0);
            entity.timeUntilNextSpawn = entityJson.getFloat("spawn_time", 0.5f);
            config.spawnableEntities.push_back(entity);
        }
        _configs.push_back(config);
    }
}

bool    Spawner::isActive() const
{
    return (this->_active);
}

void    Spawner::setActive(bool active)
{
    this->_active = active;
}

/**
    The Spawner retrieves all the configs associated to the currentWave,
    then sets itself active.
*/
void    Spawner::triggerSpawnerConfigs(uint32_t currentWave)
{
    for (auto& config : _configs)
    {
        // TODO: Handle multiple wave configs for the same wave
        if (config.associatedWave == currentWave)
        {
            this->_currentConfigs.push_back(&config);
            break;
        }
    }

    this->setActive(true);
}

void        Spawner::clearSpawnerConfigs()
{
    this->_currentConfigs.clear();
}


void    Spawner::updateClosestPath()
{
    glm::ivec2 spawnerPos;
    glm::ivec2 target(_gameManager->map.getCastlePos());

    spawnerPos.x = static_cast<int>(std::ceil(_transform->getPos().x) / 25.0f);
    spawnerPos.y = static_cast<int>(_transform->getPos().z / 25.0f);
    getPath(spawnerPos, target, _closestPath);

    updateEnemiesPaths();
}

bool        Spawner::isReadyForNextWave()
{
    auto&   it = this->_currentConfigs.begin();

    for (it; it != this->_currentConfigs.end(); ++it)
    {
        Spawner::sConfig*   waveConfig = *it;

        if (waveConfig->areAllSpawnedEntitiesDead() == false)
            return (false);
    }

    return (this->isActive() == false);
}

void    Spawner::setEnemyPath(Entity* enemy, const std::vector<glm::vec3>& path)
{
    auto scriptComponent = enemy->getComponent<sScriptComponent>();

    if (!scriptComponent)
    {
        LOG_WARN("Can't find scriptComponent on entity");
        return;
    }

    Enemy*  enemyScript = scriptComponent->getScript<Enemy>("Enemy");

    if (!enemyScript)
    {
        LOG_WARN("Can't find Enemy script on entity");
        return;
    }

    enemyScript->setPath(path);
}

void    Spawner::getPath(const glm::ivec2& from, const glm::ivec2& to, std::vector<glm::vec3>& savedPath)
{
    std::vector<glm::ivec2> path = _path.goToTarget(from,
                                                    to,
                                                    &_gameManager->map);

    savedPath.clear();
    auto& mapEntities = _gameManager->map.getEntities();
    auto& spawnersPaths = _gameManager->map.getSpawnersPaths();
    for (glm::ivec2& pos: path)
    {
        Entity* tile = mapEntities[pos.x][pos.y];
        sTransformComponent* tileTransform = tile->getComponent<sTransformComponent>();
        savedPath.push_back(tileTransform->getPos());

        spawnersPaths[pos.x][pos.y] = 1;
    }
}

void    Spawner::updateEnemiesPaths()
{
    glm::ivec2 target(_gameManager->map.getCastlePos());
    auto em = EntityFactory::getBindedEntityManager();
    for (sConfig* waveConfig: _currentConfigs)
    {
        for (const Entity::sHandle& entityHandle: waveConfig->spawnedEntities)
        {
            Entity* entity = em->getEntity(entityHandle);
            if (!entity)
            {
                continue;
            }
            glm::ivec2 entityPos;
            sTransformComponent* entityTransform = entity->getComponent<sTransformComponent>();
            std::vector<glm::vec3> enemyPath;
            entityPos.x = static_cast<int>(std::ceil(entityTransform->getPos().x) / 25.0f);
            entityPos.y = static_cast<int>(entityTransform->getPos().z / 25.0f);
            getPath(entityPos, target, enemyPath);
            setEnemyPath(entity, enemyPath);
        }
    }
}

Entity*    Spawner::spawnEntity(const std::string& entityName)
{
    auto& pos = _transform->getPos();
    Entity* enemy = this->Instantiate(entityName, glm::vec3(pos.x, 18.75, pos.z));

    setEnemyPath(enemy, _closestPath);

    return (enemy);
}

void    Spawner::spawnEntitiesFromConfig(sConfig* waveConfig, float dt)
{
    for (Spawner::sConfig::sEntity& entity : waveConfig->spawnableEntities)
    {
        entity.elapsedTime += dt;
        if (entity.amountSpawned < entity.spawnAmount &&
            entity.elapsedTime >= entity.timeUntilNextSpawn)
        {
            Entity* spawnedEntity = this->spawnEntity(entity.name);

            if (spawnedEntity != nullptr)
            {
                entity.elapsedTime = 0.0f;
                entity.amountSpawned++;
            }
            else
                LOG_ERROR("Could not spawn entity from config \"%s\"", entity.name);

            waveConfig->spawnedEntities.push_back(spawnedEntity->handle);
        }
    }
}

/**
    This function updates the 'spawned entities' pool of a config by
    removing the entities' ids by checking if they still exist in
    the EntityManager or not.
*/
void    Spawner::sConfig::updateSpawnedEntities()
{
    auto    em = EntityFactory::getBindedEntityManager();
    auto&   it = spawnedEntities.begin();

    for (it; it != spawnedEntities.end();)
    {
        Entity::sHandle    entityHandle = *it;

        //  The entity of the wave config is dead
        if (em->getEntity(entityHandle) == nullptr)
            it = spawnedEntities.erase(it);
        else
            ++it;
    }
}

bool    Spawner::sConfig::areAllEntitiesSpawned()
{
    for (auto& entity : spawnableEntities)
    {
        if (entity.amountSpawned < entity.spawnAmount)
            return (false);
    }
    return (true);
}

bool    Spawner::sConfig::areAllSpawnedEntitiesDead()
{
    return (this->areAllEntitiesSpawned() && this->spawnedEntities.size() == 0);
}
