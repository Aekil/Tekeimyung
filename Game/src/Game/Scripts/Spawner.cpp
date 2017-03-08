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

Entity* Spawner::getWaveManager() const
{
    return (this->_parent);
}

std::vector<SpawnerConfiguration>   Spawner::getSpawnerConfigs() const
{
    return (this->_configs);
}

void    Spawner::setWaveManager(Entity* waveManager)
{
    if (waveManager->getTag() != "WaveManager")
        LOG_INFO("TG noob c'est pas un WaveManager.");
    else
        this->_parent = waveManager;
}

void    Spawner::addSpawnerConfig(SpawnerConfiguration config)
{
    if (this->_parent != nullptr)
        LOG_INFO("TG noob tu as besoin d'un WaveManager.");
    else
    {
        sScriptComponent*   script = this->_parent->getComponent<sScriptComponent>();
        WaveManager*        waveManagerScript = script->getScript<WaveManager>("WaveManager");
        int configWave = config.getAssociatedWave();

        if (configWave <= 0 || configWave > waveManagerScript->getWaves())
            LOG_INFO("TG noob spécifie une bonne wave, gros bâtard.");
        else
            this->_configs.push_back(config);
    }
}

void    Spawner::deleteSpawnerConfig(int index)
{
    this->_configs.erase(this->_configs.begin() + index);
}