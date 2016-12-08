#pragma once

#include    <ECS/System.hpp>
#include    <ECS/World.hpp>

#include    <Engine/Core/GameState.hpp>

#include    <Game/EntityFactory.hpp>

START_GAMESTATE(OptionsMenuState)
 public:
     virtual        ~OptionsMenuState();

     virtual void   onEnter();
     virtual bool   init();
     virtual bool   update(float elapsedTime);

     void           initCamera();

     Entity*        createButton(eArchetype type, const glm::vec2& pos);
     void           handleButtons();

private:
    System*         _optionsMenuStateRenderSystem;
    World*          _optionsMenuStateWorld;

    Camera          _camera;

    Entity*         _toggleFullsreenButton;
    Entity*         _returnButton;
END_GAMESTATE(OptionsMenuState)
