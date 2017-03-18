/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(PauseState, "Pause")
 public:
    ~PauseState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;
END_GAMESTATE(PauseState)
