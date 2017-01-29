/**
* @Author   Guillaume Labey
*/

#pragma once

#include <utility>

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

START_GAMESTATE(ConfirmExitState, "ConfirmExit")
 public:
    ~ConfirmExitState();

    void                                onEnter() override final;
    void                                setupSystems() override final;
    bool                                init() override final;
    bool                                update(float elapsedTime) override final;

    Entity*                             createButton(eArchetype type, const glm::vec2& pos);
    bool                                handleButtons();

private:
    Entity*                             _yesButton = nullptr;
    Entity*                             _noButton = nullptr;
END_GAMESTATE(ConfirmExitState)
