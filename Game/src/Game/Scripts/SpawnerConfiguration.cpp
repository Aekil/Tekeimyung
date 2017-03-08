/*
** @Author : Mathieu CHASSARA
*/

#include    <Game/Scripts/SpawnerConfiguration.hpp>

Entity*     SpawnerConfiguration::getSpawnableEntity() const
{
    return (this->_spawnableEntity);
}

int         SpawnerConfiguration::getSpawnAmount() const
{
    return (this->_spawnAmount);
}

float       SpawnerConfiguration::getTimeUntilNextSpawn() const
{
    return (this->_timeUntilNextSpawn);
}

int         SpawnerConfiguration::getAssociatedWave() const
{
    return (this->_associatedWave);
}

void        SpawnerConfiguration::setSpawnableEntity(Entity* entity)
{
    this->_spawnableEntity = entity;
}

void        SpawnerConfiguration::setSpawnAmount(int amount)
{
    this->_spawnAmount = amount;
}

void        SpawnerConfiguration::setTimeUntilNextSpawn(float time)
{
    this->_timeUntilNextSpawn = time;
}

void        SpawnerConfiguration::setAssociatedWave(int wave)
{
    this->_associatedWave = wave;
}