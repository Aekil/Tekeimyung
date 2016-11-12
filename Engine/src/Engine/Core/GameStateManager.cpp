#include <Engine/Core/GameStateManager.hpp>

GameStateManager::GameStateManager() {}

GameStateManager::~GameStateManager() {}

void    GameStateManager::removeCurrentState()
{
    if (!_states.empty())
    {
        _states.pop_back();
    }
}

std::shared_ptr<GameState> GameStateManager::getCurrentState() const
{
    return (_states.back());
}

bool    GameStateManager::hasStates() const
{
    return (_states.size() != 0);
}
