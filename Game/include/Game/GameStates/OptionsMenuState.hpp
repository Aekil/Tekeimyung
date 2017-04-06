/**
* @Author   Mathieu Chassara
*/

#pragma once

#include    <ECS/System.hpp>
#include    <ECS/World.hpp>

#include    <Engine/Core/GameState.hpp>

#include    <Engine/EntityFactory.hpp>

START_GAMESTATE(OptionsMenuState, "Options")
 public:
     ~OptionsMenuState();

    void            onEnter() override final;
    void            setupSystems() override final;
    bool            init() override final;
    bool            update(float elapsedTime) override final;

    bool            handleButtons();

private:
    Entity*         _buttonToggleWindowed = nullptr;
    Entity*         _buttonToggleFullscreen = nullptr;
    Entity*         _buttonCurrentWindowMode = nullptr;

    void            createOrGetButtons();
END_GAMESTATE(OptionsMenuState)
