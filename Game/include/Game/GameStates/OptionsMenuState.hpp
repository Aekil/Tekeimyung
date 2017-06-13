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
    //Entity*         _buttonToggleWindowed = nullptr;
    //Entity*         _buttonToggleFullscreen = nullptr;
    
    //Entity*         _buttonToggleVolume = nullptr;
    //Entity*         _buttonMuteVolume = nullptr;

    Entity*         _buttonCurrentWindowMode = nullptr;
    Entity*         _buttonCurrentVolumeMode = nullptr;

    void            createOrGetButtons();
    bool            toggleVolume();
END_GAMESTATE(OptionsMenuState)
