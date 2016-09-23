#include "Core/GameState.hpp"
#include "Utils/EngineException.hpp"

bool    GameState::initSystems()
{
    for (auto system: _world.getSystems())
    {
        if (!system->init())
        {
            return (false);
        }
    }

    return (true);
}

bool    GameState::update(float elapsedTime)
{
    try
    {
        for (auto system: _world.getSystems())
        {
            system->update(_world.getEntityManager(), elapsedTime);
        }
    }
    catch(const EngineException &e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    return (true);
}