#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Timer.hpp>

#include <Game/Components.hh>
#include <Game/Utils/PlayStates.hpp>

#include <Game/Systems/WaveSystem.hpp>

WaveSystem::WaveSystem(Map* map) : _map(map)
{
    addDependency<sWaveComponent>();
    addDependency<sPositionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(WAVE_SYSTEM_NAME);
}

WaveSystem::~WaveSystem() {}

void    WaveSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    bool reset = false;

    forEachEntity(em, [&](Entity *entity)
    {
        sWaveComponent* waveComponent = entity->getComponent<sWaveComponent>();
        if (waveComponent)
        {
            if (!waveComponent->firstWait)
            {
                if (waveComponent->timer.getElapsedTime() > waveComponent->secBeforeFirstSpawn)
                {
                    waveComponent->timer.reset();
                    waveComponent->firstWait = true;
                }
            }
            if (waveComponent->firstWait)
            {
                sPositionComponent* position = entity->getComponent<sPositionComponent>();

                waveComponent->spawnPos = glm::vec3(position->value.x + 0.5f, position->value.y + 0.5f, position->z + 1);
                if (waveComponent->nbEntities > 0 && waveComponent->timer.getElapsedTime() > waveComponent->secBeforeEachSpawn)
                {
                    waveComponent->timer.reset();
                    createEntityFromWave(_map, waveComponent->spawnPos, eArchetype::ENEMY);
                    waveComponent->nbEntities -= 1;
                    if (waveComponent->nbEntities == 0)
                        em.destroyEntityRegister(entity);
                }
            }
        }
    });

    _monitoringData.timeSec = timer.getElapsedTime();
    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, _monitoringData);
}

uint32_t    WaveSystem::createWave(Map* map, const glm::vec3& pos, eArchetype type)
{
    Entity* waveEntity;
    
    waveEntity = PlayStates::createTile(map, pos, type);
    
    return (waveEntity->id);
}

void     WaveSystem::setNbEntities(EntityManager &em, uint32_t waveEntityID, int entityNb)
{
    Entity *waveEntity;
    
    waveEntity = em.getEntity(waveEntityID);
    sWaveComponent* wave = waveEntity->getComponent<sWaveComponent>();
    wave->nbEntities = entityNb;
}

void     WaveSystem::setSecBeforeFirstSpawn(EntityManager &em, uint32_t waveEntityID, float secBeforeFirstSpawn)
{
    Entity *waveEntity;

    waveEntity = em.getEntity(waveEntityID);
    sWaveComponent* wave = waveEntity->getComponent<sWaveComponent>();
    wave->secBeforeFirstSpawn = secBeforeFirstSpawn;
}

void     WaveSystem::setSecBeforeEachSpawn(EntityManager &em, uint32_t waveEntityID, float secBeforeEachSpawn)
{
    Entity *waveEntity;

    waveEntity = em.getEntity(waveEntityID);
    sWaveComponent* wave = waveEntity->getComponent<sWaveComponent>();
    wave->secBeforeEachSpawn = secBeforeEachSpawn;
}

void     WaveSystem::setAllFields(EntityManager &em, uint32_t waveEntityID, sWaveComponent &waveData)
{
    Entity *waveEntity;

    waveEntity = em.getEntity(waveEntityID);
    sWaveComponent* wave = waveEntity->getComponent<sWaveComponent>();
    wave->nbEntities = waveData.nbEntities;
    wave->secBeforeFirstSpawn = waveData.secBeforeFirstSpawn;
    wave->secBeforeEachSpawn = waveData.secBeforeEachSpawn;
}

Entity*    WaveSystem::createEntityFromWave(Map* map, const glm::vec3& pos, eArchetype type)
{
    Entity* entity = EntityFactory::createEntity(type);

    sPositionComponent* posEntity = entity->getComponent<sPositionComponent>();
    posEntity->value.x = pos.x;
    posEntity->value.y = pos.y;
    posEntity->z = pos.z;

    (*map)[(uint16_t)pos.z].addEntity(entity->id);

    /*  static int idSoundSpawn = SoundManager::getInstance()->registerSound("resources/sounds/spawn.mp3", DEFAULT_SOUND);
    SoundManager::getInstance()->playSound(idSoundSpawn);*/

    return (entity);
}