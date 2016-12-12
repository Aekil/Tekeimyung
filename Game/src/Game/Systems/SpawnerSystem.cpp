#include <glm/gtx/string_cast.hpp>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Timer.hpp>
#include <Engine/Sound/SoundManager.hpp>

#include <Game/Components.hh>
#include <Game/Utils/EventSound.hpp>
#include <Game/Utils/PlayStates.hpp>

#include <Game/Systems/SpawnerSystem.hpp>

SpawnerSystem::SpawnerSystem(Map* map) : _map(map)
{
    addDependency<sSpawnerComponent>();
    addDependency<sPositionComponent>();

    _monitoringKey = MonitoringDebugWindow::getInstance()->registerSystem(SPAWNER_SYSTEM_NAME);
}

SpawnerSystem::~SpawnerSystem() {}

void    SpawnerSystem::update(EntityManager &em, float elapsedTime)
{
    Timer timer;
    uint32_t nbEntities = 0;
    bool skipTimeFirstSpawn = false;
    static bool caca = false;

    forEachEntity(em, [&](Entity *entity)
    {
        sSpawnerComponent* spawnerComponent = entity->getComponent<sSpawnerComponent>();
        if (spawnerComponent)
        {
            spawnerComponent->timeRec += elapsedTime; // update time record
            if (!spawnerComponent->firstWaitFinished) // first spawn time isn't finished
            {
#if (ENABLE_SOUND)
                float tmpTimeBeforeStart = (spawnerComponent->data.secBeforeFirstSpawn - spawnerComponent->timeRec);
                if (!caca && tmpTimeBeforeStart >= 1.5 && tmpTimeBeforeStart <= 2.5)
                {
                    caca = true;
                    /*static int idWaveStartEffect = SoundManager::getInstance()->registerSound("resources/sounds/countdown5.mp3", DEFAULT_SOUND);
                    SoundManager::getInstance()->playSound(idWaveStartEffect);*/

                    if (EventSound::isEventLinkedToSound(eEventSound::COUNTDOWN_WAVE))
                    {
                        SoundManager::getInstance()->playSound(EventSound::getSoundIDFromEvent(eEventSound::COUNTDOWN_WAVE));
                    }
                }
#endif
                if (spawnerComponent->timeRec > spawnerComponent->data.secBeforeFirstSpawn)
                {
                    skipTimeFirstSpawn = true; // remember to skip time condition (/additionnals time) for first spawn
                    spawnerComponent->firstWaitFinished = true;
                    spawnerComponent->timeRec = 0; // reset time record here to correctly wait time until next spawn
                }
            }
            if (spawnerComponent->firstWaitFinished) // first spawn time is finished
            {
                sPositionComponent* position = entity->getComponent<sPositionComponent>();

                spawnerComponent->spawnPos = glm::vec3(position->value.x + 0.5f, position->value.y + 0.5f, position->z); // center the spawn point
                // if there is entities to spawn & ( enough waited time OR first spawn time is skipped)
                if (spawnerComponent->data.nbEntities > 0 &&
                    (skipTimeFirstSpawn || spawnerComponent->timeRec > spawnerComponent->data.secBeforeEachSpawn))
                {
                    spawn(em, entity, spawnerComponent, &skipTimeFirstSpawn);
                }
            }
        }
        ++nbEntities;
    });

    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, timer.getElapsedTime(), nbEntities);
}

uint32_t    SpawnerSystem::createSpawner(Map* map, const glm::vec3& pos, eArchetype type)
{
    Entity* spawnerEntity;

    spawnerEntity = PlayStates::createTile(map, pos, type);

    return (spawnerEntity->id);
}

void     SpawnerSystem::setNbEntities(EntityManager &em, uint32_t spawnerEntityID, int entityNb)
{
    Entity *spawnerEntity;

    spawnerEntity = em.getEntity(spawnerEntityID);
    sSpawnerComponent* spawner = spawnerEntity->getComponent<sSpawnerComponent>();
    spawner->data.nbEntities = entityNb;
}

void     SpawnerSystem::setSecBeforeFirstSpawn(EntityManager &em, uint32_t spawnerEntityID, float secBeforeFirstSpawn)
{
    Entity *spawnerEntity;

    spawnerEntity = em.getEntity(spawnerEntityID);
    sSpawnerComponent* spawner = spawnerEntity->getComponent<sSpawnerComponent>();
    spawner->data.secBeforeFirstSpawn = secBeforeFirstSpawn;
}

void     SpawnerSystem::setSecBeforeEachSpawn(EntityManager &em, uint32_t spawnerEntityID, float secBeforeEachSpawn)
{
    Entity *spawnerEntity;

    spawnerEntity = em.getEntity(spawnerEntityID);
    sSpawnerComponent* spawner = spawnerEntity->getComponent<sSpawnerComponent>();
    spawner->data.secBeforeEachSpawn = secBeforeEachSpawn;
}

void     SpawnerSystem::setSecBeforeLastSpawn(EntityManager &em, uint32_t spawnerEntityID, float secBeforeLastSpawn)
{
    Entity *spawnerEntity;

    spawnerEntity = em.getEntity(spawnerEntityID);
    sSpawnerComponent* spawner = spawnerEntity->getComponent<sSpawnerComponent>();
    spawner->data.secBeforeLastSpawn = secBeforeLastSpawn;
}

void     SpawnerSystem::setAllFields(EntityManager &em, uint32_t spawnerEntityID, tSpawnerData &spawnerData)
{
    Entity *spawnerEntity;

    spawnerEntity = em.getEntity(spawnerEntityID);
    sSpawnerComponent* spawner = spawnerEntity->getComponent<sSpawnerComponent>();
    spawner->data.nbEntities = spawnerData.nbEntities;
    spawner->data.secBeforeFirstSpawn = spawnerData.secBeforeFirstSpawn;
    spawner->data.secBeforeEachSpawn = spawnerData.secBeforeEachSpawn;
    spawner->data.secBeforeLastSpawn = spawnerData.secBeforeLastSpawn;
}

tSpawnerData*  SpawnerSystem::getStructData(int nbEnt, float firstTime, float eachTime, float lastTime)
{
    tSpawnerData *spawnerData = new tSpawnerData{ nbEnt, firstTime, eachTime, lastTime };

    return (spawnerData);
}

Entity*    SpawnerSystem::createEntityFromSpawner(Map* map, const glm::vec3& pos, eArchetype type)
{
    Entity* entity = EntityFactory::createEntity(type, pos);

    (*map)[(uint16_t)pos.z].addEntity(entity->id);

#if (ENABLE_SOUND)
    /*static int idSoundSpawn = SoundManager::getInstance()->registerSound("resources/sounds/spawn.mp3", DEFAULT_SOUND);
    SoundManager::getInstance()->playSound(idSoundSpawn);*/

#endif
    return (entity);
}

void    SpawnerSystem::spawn(EntityManager &em, Entity* entity, sSpawnerComponent* spawnerComponent, bool *skipTimeFirstSpawn)
{
    *skipTimeFirstSpawn = false; // disable skip of waiting time
    spawnerComponent->timeRec = 0; // reset time record for next spawn
    createEntityFromSpawner(_map, spawnerComponent->spawnPos, eArchetype::ENEMY); // spawn entity
    spawnerComponent->data.nbEntities -= 1; // count number of entities left to spawn
    if (spawnerComponent->data.nbEntities == 0) // if no more entities left to spawn
        em.destroyEntityRegister(entity); // detroy spawner entity
}
