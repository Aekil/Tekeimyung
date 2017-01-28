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

    void                                initPreviousStatesRender();

    Entity*                             createButton(eArchetype type, const glm::vec2& pos);
    void                                handleButtons();

private:
    std::vector<System*>                _previousStatesSystems;
    std::vector<World*>                 _previousStatesWorlds;

    Camera                              _camera;

    Entity*                             _yesButton = nullptr;
    Entity*                             _noButton = nullptr;
END_GAMESTATE(ConfirmExitState)
