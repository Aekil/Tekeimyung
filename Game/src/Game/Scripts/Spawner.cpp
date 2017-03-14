/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Spawner.hpp>
#include <Game/Scripts/WaveManager.hpp>

void Spawner::start()
{
    auto& pos = this->getComponent<sTransformComponent>()->getPos();

    this->Instantiate("ENEMY", glm::vec3(pos.x, 18.75, pos.z));
}

void Spawner::update(float dt)
{

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
            changed |= ImGui::InputInt("Amount", &entity.spawnAmount, 5, ImGuiInputTextFlags_AllowTabInput);
            changed |= ImGui::InputFloat("Spawn time", &entity.timeUntilNextSpawn, 0.5f, ImGuiInputTextFlags_AllowTabInput);
            ImGui::PopID();
            i++;
        }
    }

    return (changed);
}

Entity* Spawner::getWaveManager() const
{
    return (this->_parent);
}

void    Spawner::setWaveManager(Entity* waveManager)
{
    if (waveManager->getTag() != "WaveManager")
        LOG_INFO("TG noob c'est pas un WaveManager.");
    else
        this->_parent = waveManager;
}

/*void    Spawner::addSpawnerConfig(SpawnerConfiguration config)
{
    if (this->_parent != nullptr)
        LOG_INFO("TG noob tu as besoin d'un WaveManager.");
    else
    {
        sScriptComponent*   script = this->_parent->getComponent<sScriptComponent>();
        WaveManager*        waveManagerScript = script->getScript<WaveManager>("WaveManager");

        if (config.associatedWave <= 0 || config.associatedWave > waveManagerScript->getWaves())
            LOG_INFO("TG noob spécifie une bonne wave, gros bâtard.");
        else
            this->_configs.push_back(config);
    }
}*/

void    Spawner::deleteSpawnerConfig(int index)
{
    this->_configs.erase(this->_configs.begin() + index);
}
