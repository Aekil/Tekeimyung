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
    bool skipTimeFirstSpawn = false;

    forEachEntity(em, [&](Entity *entity)
    {
        sWaveComponent* waveComponent = entity->getComponent<sWaveComponent>();
        if (waveComponent)
        {
            waveComponent->timeRec += elapsedTime; // update time record
            if (!waveComponent->firstWaitFinished) // first spawn time isn't finished
            {
                if (waveComponent->timeRec > waveComponent->data.secBeforeFirstSpawn)
                {
                    skipTimeFirstSpawn = true; // remember to skip time condition (/additionnals time) for first spawn
                    waveComponent->firstWaitFinished = true;
                    waveComponent->timeRec = 0; // reset time record here to correctly wait time until next spawn
                }
            }
            if (waveComponent->firstWaitFinished) // first spawn time is finished
            {
                sPositionComponent* position = entity->getComponent<sPositionComponent>();

                waveComponent->spawnPos = glm::vec3(position->value.x + 0.5f, position->value.y + 0.5f, position->z + 1); // center the spawn point
                // if there is entities to spawn & ( enough waited time OR first spawn time is skipped)
                if (waveComponent->data.nbEntities > 0 &&
                    (skipTimeFirstSpawn || waveComponent->timeRec > waveComponent->data.secBeforeEachSpawn))
                {
                    skipTimeFirstSpawn = false; // disable skip of waiting time
                    waveComponent->timeRec = 0; // reset time record for next spawn
                    createEntityFromWave(_map, waveComponent->spawnPos, eArchetype::ENEMY); // spawn entity
                    waveComponent->data.nbEntities -= 1; // count number of entities left to spawn
                    if (waveComponent->data.nbEntities == 0) // if no more entities left to spawn
                        em.destroyEntityRegister(entity); // detroy wave entity
                }
            }
        }
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime());
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
    wave->data.nbEntities = entityNb;
}

void     WaveSystem::setSecBeforeFirstSpawn(EntityManager &em, uint32_t waveEntityID, float secBeforeFirstSpawn)
{
    Entity *waveEntity;

    waveEntity = em.getEntity(waveEntityID);
    sWaveComponent* wave = waveEntity->getComponent<sWaveComponent>();
    wave->data.secBeforeFirstSpawn = secBeforeFirstSpawn;
}

void     WaveSystem::setSecBeforeEachSpawn(EntityManager &em, uint32_t waveEntityID, float secBeforeEachSpawn)
{
    Entity *waveEntity;

    waveEntity = em.getEntity(waveEntityID);
    sWaveComponent* wave = waveEntity->getComponent<sWaveComponent>();
    wave->data.secBeforeEachSpawn = secBeforeEachSpawn;
}

void     WaveSystem::setAllFields(EntityManager &em, uint32_t waveEntityID, tWaveData &waveData)
{
    Entity *waveEntity;

    waveEntity = em.getEntity(waveEntityID);
    sWaveComponent* wave = waveEntity->getComponent<sWaveComponent>();
    wave->data.nbEntities = waveData.nbEntities;
    wave->data.secBeforeFirstSpawn = waveData.secBeforeFirstSpawn;
    wave->data.secBeforeEachSpawn = waveData.secBeforeEachSpawn;
}

tWaveData*  WaveSystem::getStructData(int nbEnt, float firstTime, float eachTime)
{
    tWaveData *waveData = new tWaveData{ nbEnt, firstTime, eachTime };

    return (waveData);
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