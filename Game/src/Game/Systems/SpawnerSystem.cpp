#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Timer.hpp>

#include <Game/Components.hh>
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
    bool skipTimeFirstSpawn = false;

    forEachEntity(em, [&](Entity *entity)
    {
        sSpawnerComponent* spawnerComponent = entity->getComponent<sSpawnerComponent>();
        if (spawnerComponent)
        {
            if (!spawnerComponent->firstWaitFinished) // first spawn time isn't finished
            {
                if (spawnerComponent->timer.getElapsedTime() > spawnerComponent->data.secBeforeFirstSpawn)
                {
                    skipTimeFirstSpawn = true; // remember to skip time condition (/additionnals time) for first spawn
                    spawnerComponent->firstWaitFinished = true;
                    spawnerComponent->timer.reset(); // reset timer here to correctly wait time until next spawn
                }
            }
            if (spawnerComponent->firstWaitFinished) // first spawn time is finished
            {
                sPositionComponent* position = entity->getComponent<sPositionComponent>();

                spawnerComponent->spawnPos = glm::vec3(position->value.x + 0.5f, position->value.y + 0.5f, position->z + 1); // center the spawn point
                // if there is entities to spawn & ( enough waited time OR first spawn time is skipped)
                if (spawnerComponent->data.nbEntities == 1)
                {
                    if (skipTimeFirstSpawn || spawnerComponent->timer.getElapsedTime() > spawnerComponent->data.secBeforeLastSpawn)
                    {
                        spawn(em, entity, spawnerComponent, &skipTimeFirstSpawn);
                    }
                }
                else if (spawnerComponent->data.nbEntities > 0 &&
                    (skipTimeFirstSpawn || spawnerComponent->timer.getElapsedTime() > spawnerComponent->data.secBeforeEachSpawn))
                {
                    spawn(em, entity, spawnerComponent, &skipTimeFirstSpawn);
                }
            }
        }
    });

    _monitoringData.timeSec = timer.getElapsedTime();
    MonitoringDebugWindow::getInstance()->updateSystem(_monitoringKey, _monitoringData);
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

void    SpawnerSystem::spawn(EntityManager &em, Entity* entity, sSpawnerComponent* spawnerComponent, bool *skipTimeFirstSpawn)
{
    *skipTimeFirstSpawn = false; // disable skip of waiting time
    spawnerComponent->timer.reset(); // reset timer for next spawn
    createEntityFromSpawner(_map, spawnerComponent->spawnPos, eArchetype::ENEMY); // spawn entity
    spawnerComponent->data.nbEntities -= 1; // count number of entities left to spawn
    if (spawnerComponent->data.nbEntities == 0) // if no more entities left to spawn
        em.destroyEntityRegister(entity); // detroy spawner entity
}