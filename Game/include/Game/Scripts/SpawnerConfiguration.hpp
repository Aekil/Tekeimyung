/*
** @Author : Mathieu CHASSARA
*/

#include    <ECS/Entity.hpp>

class       SpawnerConfiguration final
{
public:
    SpawnerConfiguration() = default;
    ~SpawnerConfiguration() = default;

private:
    Entity* _spawnableEntity;
    int     _spawnAmount;
    float   _timeUntilNextSpawn;
    int     _associatedWave;

public:
    Entity* getSpawnableEntity() const;
    int     getSpawnAmount() const;
    float   getTimeUntilNextSpawn() const;
    int     getAssociatedWave() const;

    void    setSpawnableEntity(Entity* entity);
    void    setSpawnAmount(int amount);
    void    setTimeUntilNextSpawn(float time);
    void    setAssociatedWave(int wave);
};