/**
* @Author   Guillaume Labey
*/

#include <Engine/Core/GameStateManager.hpp>
#include <Engine/Debug/Logger.hpp>

GameStateManager::GameStateManager() {}

GameStateManager::~GameStateManager() {}

bool    GameStateManager::removeCurrentState()
{
    if (!hasStates())
    {
        return (false);
    }

    _states.pop_back();
    if (hasStates())
    {
        _states.back()->bindEntityManager();
        _states.back()->onEnter();
    }

    return (true);
}

std::shared_ptr<GameState> GameStateManager::getCurrentState() const
{
    if (hasStates())
        return (_states.back());
    return (nullptr);
}

bool    GameStateManager::hasStates() const
{
    return (_states.size() != 0);
}

void    GameStateManager::clearStates()
{
    _states.clear();
}

void    GameStateManager::removeLastStates(uint32_t nb)
{
    if (!nb)
    {
        return;
    }
    else if (nb > _states.size())
    {
        LOG_WARN("Can't remove %d states because the stack contains only %d states", (int)nb, (int)_states.size());
        return;
    }

    _states.erase(_states.end() - nb, _states.end());
}
