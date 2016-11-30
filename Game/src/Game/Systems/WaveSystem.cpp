#include <Engine/Utils/Debug.hpp>

#include <Game/Components.hh>
#include <Game/Utils/PlayStates.hpp>

#include <Game/Systems/WaveSystem.hpp>

WaveSystem::WaveSystem(Map* map) : _map(map)
{
    _timer = new Timer();
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
            if (waveComponent->firstWait || _timer->getElapsedTime() > waveComponent->secBeforeFirstSpawn)
            {
                if (!waveComponent->firstWait)
                {
                    reset = true; // if you want to wait the time between each spawn in addition of the first one, just replace this line by "_timer->reset();"
                    waveComponent->firstWait = true;
                }
                sPositionComponent* position = entity->getComponent<sPositionComponent>();

                waveComponent->spawnPos = glm::vec3(position->value.x + 0.5f, position->value.y + 0.5f, position->z + 1);
                if (waveComponent->nSpawn > 0 && _timer->getElapsedTime() > waveComponent->secBeforeEachSpawn)
                {
                    createEntityFromWave(_map, waveComponent->spawnPos, eArchetype::ENEMY);
                    waveComponent->nSpawn -= 1;
                    reset = true;
                }
            }
        }
    });
    if (reset)
        _timer->reset();

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
    wave->nSpawn = entityNb;
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