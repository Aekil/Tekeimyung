/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>
#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/Spawner.hpp>
#include <Game/Scripts/WaveManager.hpp>

void Spawner::start()
{
    auto em = EntityFactory::getBindedEntityManager();
    _transform = this->getComponent<sTransformComponent>();

    // Init closest path
    {
        Entity* gameManager = em->getEntityByTag("GameManager");
        if (!gameManager)
        {
            LOG_WARN("Can't find entity with GameManager tag");
            return;
        }

        auto scriptComponent = gameManager->getComponent<sScriptComponent>();

        if (!scriptComponent)
        {
            LOG_WARN("Can't find scriptComponent on GameManager entity");
            return;
        }

        _gameManager = scriptComponent->getScript<GameManager>("GameManager");
        _closestPath = getClosestPath();
    }
}

void Spawner::update(float dt)
{
    auto& it = _currentWaves.begin();
    for (it; it != _currentWaves.end();)
    {
        sConfig* waveConfig = *it;

        waveConfig->updateSpawnedEntities();
        spawnEntities(waveConfig, dt);

        if ((waveConfig->spawnedEntities.size() == 0 && // All spawned entities are dead
            waveConfig->allEntitiesSpawned()) ||
            waveConfig->spawnableEntities.size() == 0) // The spawner has not spawnable entities
            {
                it = _currentWaves.erase(it);
            }
        else
            ++it;
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
        static std::vector<const char*> spawnableEntityTypes = {"ENEMY"};

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

void    Spawner::startWave(uint32_t waveNb)
{
    for (auto& config: _configs)
    {
        // TODO: Handle multiple wave configs for the same wave
        if (config.associatedWave == waveNb)
        {
            _currentWaves.push_back(&config);
            break;
        }
    }
}

bool    Spawner::checkEndWave()
{
    return (_currentWaves.size() == 0);
}

std::vector<glm::vec3> Spawner::getClosestPath() const
{
    if (!_gameManager ||
        _gameManager->paths.size() == 0)
    {
        return (std::vector<glm::vec3>());
    }

    float closestDistance = 99999999999999.9f;
    std::vector<glm::vec3> closestPath = _gameManager->paths[0];
    for (auto path: _gameManager->paths)
    {
        if (path.size() == 0)
            continue;
        float distance = glm::distance(_transform->getPos(), path[0]);
        if (distance < closestDistance)
        {
            closestPath = path;
            closestDistance = distance;
        }
    }

    return (closestPath);
}

Entity*    Spawner::spawnEntity(const std::string& entityName)
{
    auto& pos = _transform->getPos();
    Entity* enemy = this->Instantiate(entityName, glm::vec3(pos.x, 18.75, pos.z));
    auto scriptComponent = enemy->getComponent<sScriptComponent>();

    if (!scriptComponent)
    {
        LOG_WARN("Can't find scriptComponent on %s entity", entityName.c_str());
        return (enemy);
    }

    Enemy* enemyScript = scriptComponent->getScript<Enemy>("Enemy");

    if (!enemyScript)
    {
        LOG_WARN("Can't find Enemy script on %s entity", entityName.c_str());
        return (enemy);
    }
    enemyScript->setPath(_closestPath);

    return (enemy);
}

void    Spawner::spawnEntities(sConfig* waveConfig, float dt)
{
    for (auto& entity: waveConfig->spawnableEntities)
    {
        entity.elapsedTime += dt;
        if ((!entity.spawnedNb || entity.elapsedTime >= entity.timeUntilNextSpawn) && // First spawn or timeUntilNextSpawn
            entity.spawnedNb < entity.spawnAmount)
        {
            entity.elapsedTime = 0.0f;
            entity.spawnedNb++;
            Entity* spawnedEntity = spawnEntity(entity.name);
            if (!spawnedEntity)
            {
                LOG_ERROR("Cannot spawn entity");
                continue;
            }
            waveConfig->spawnedEntities.push_back(spawnedEntity->id);
        }
    }
}

void    Spawner::sConfig::updateSpawnedEntities()
{
    auto em = EntityFactory::getBindedEntityManager();
    auto& it = spawnedEntities.begin();
    for (it; it != spawnedEntities.end();)
    {
        uint32_t entityId = *it;
        // The entity of the wave config is dead
        if (!em->getEntity(entityId))
        {
            it = spawnedEntities.erase(it);
        }
        else
            ++it;
    }
}

bool Spawner::sConfig::allEntitiesSpawned()
{
    bool allSpawned = true;
    for (auto& entity: spawnableEntities)
    {
        if (entity.spawnedNb != entity.spawnAmount)
            allSpawned = false;
    }
    return (allSpawned);
}
