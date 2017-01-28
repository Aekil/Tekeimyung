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
    System*         _playstateRenderSystem;
    World*          _playstateWorld;

    Camera          _camera;

    Entity*         _toggleWindowModeButton = nullptr;
    Entity*         _returnButton = nullptr;

    void            createToggleWindowModeButton();
END_GAMESTATE(OptionsMenuState)
