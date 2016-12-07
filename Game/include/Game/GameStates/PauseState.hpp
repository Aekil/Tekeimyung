#pragma once

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(PauseState)
 public:
    virtual ~PauseState();

    virtual void                        onEnter();
    virtual bool                        init();
    virtual bool                        update(float elapsedTime);

private:
    System*                             _playStateRendersystem;
    World*                              _playStateWorld;
END_GAMESTATE(PauseState)
