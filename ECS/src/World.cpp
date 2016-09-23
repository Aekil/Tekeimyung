#include "World.h"

World::~World()
{
    
}

EntityManager               &World::getEntityManager()
{
    return _entityManager;
}

std::vector<System*>        &World::getSystems()
{
    return _systems;
}