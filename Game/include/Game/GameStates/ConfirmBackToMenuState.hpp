/**
* @Author   Guillaume Labey
*/

#pragma once

#include <utility>

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(ConfirmBackToMenuState, "ConfirmBackToMenu")
 public:
    ~ConfirmBackToMenuState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;
END_GAMESTATE(ConfirmBackToMenuState)
